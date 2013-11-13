#pragma once

#include <d3d9.h>
#include <string>

class directx_cap
{
public:
	directx_cap();
	~directx_cap();
	bool init();
	bool get_frame_to_buffer(int size,DWORD* buffer);
	bool get_frame_to_file(const std::string& path);

	int get_disp_width()
	{
		return disp_width_;
	}
	int get_disp_height()
	{
		return disp_height_;
	}

private:
	IDirect3D9*			d3d9_ptr_ = NULL;
	IDirect3DDevice9*	d3d9_dev_ptr_ = NULL;
	IDirect3DSurface9*	d3d9_sur_ptr_ = NULL;

	int	disp_width_ = 0;
	int	disp_height_ = 0;
};

