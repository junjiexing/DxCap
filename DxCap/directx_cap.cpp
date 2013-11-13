#include <windows.h>
#include "directx_cap.h"
#pragma comment(lib,"d3d9.lib")

directx_cap::directx_cap()
{
}


directx_cap::~directx_cap()
{
	if (d3d9_sur_ptr_)
	{
		d3d9_sur_ptr_->Release();
	}
	if (d3d9_dev_ptr_)
	{
		d3d9_dev_ptr_->Release();
	}
	if (d3d9_ptr_)
	{
		d3d9_ptr_->Release();
	}
}

bool directx_cap::init()
{
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	if ((d3d9_ptr_ = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}

	if (FAILED(d3d9_ptr_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddm)))
	{
		return false;
	}

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed = TRUE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat = ddm.Format;
	d3dpp.BackBufferHeight = disp_height_ = ddm.Height;
	d3dpp.BackBufferWidth = disp_width_ = ddm.Width;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = NULL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if (FAILED(d3d9_ptr_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3d9_dev_ptr_)))
	{
		return false;
	}

	if (FAILED(d3d9_dev_ptr_->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &d3d9_sur_ptr_, NULL)))
	{
		return false;
	}

	return true;

}

bool directx_cap::get_frame_to_buffer(int size, DWORD* buffer)
{
	if (size<disp_height_*disp_width_)
	{
		return false;
	}

	d3d9_dev_ptr_->GetFrontBufferData(0, d3d9_sur_ptr_);

	D3DLOCKED_RECT	lockedRect;
	if (FAILED(d3d9_sur_ptr_->LockRect(&lockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
	{
		return false;
	}

	for (int i = 0; i < disp_height_; ++i)
	{
		BYTE* pColor = (BYTE*)lockedRect.pBits + i*lockedRect.Pitch;
		for (int j = 0; j < disp_width_; ++j)
		{
			buffer[i*disp_width_+j] = RGB(*(pColor + j * 4 + 0), *(pColor + j * 4 + 1), *(pColor + j * 4 + 2));
		}
	}
	d3d9_sur_ptr_->UnlockRect();

	return true;
}

bool directx_cap::get_frame_to_file(const std::string& path)
{
	return false;
}
