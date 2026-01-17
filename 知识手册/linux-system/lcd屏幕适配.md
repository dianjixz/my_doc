# raspberry zero 2w 适配lcd屏幕

本次适配的的屏幕时st7789，分辨率是170*320，屏幕接口是spi。  
st7789的驱动ram分辨率时240*320,重要的是屏幕是居中显示的，导致 fbtft 的st7789v 的驱动无法正常显示，需要修改驱动。

## 1. 修改驱动

修改驱动文件，在驱动文件中添加如下代码：

```c
static void fbtft_set_addr_win(struct fbtft_par *par, int xs, int ys, int xe,
			       int ye)
{
	ys += 35;
	ye += 35;
	write_reg(par, MIPI_DCS_SET_COLUMN_ADDRESS,
		  (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF);

	write_reg(par, MIPI_DCS_SET_PAGE_ADDRESS,
		  (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF);

	write_reg(par, MIPI_DCS_WRITE_MEMORY_START);
}
static struct fbtft_display display = {
	.regwidth = 8,
	.width = 240,
	.height = 320,
	.gamma_num = 2,
	.gamma_len = 14,
	.gamma = HSD20_IPS_GAMMA,
	.fbtftops = {
		.init_display = init_display,
		.write_vmem = write_vmem,
		.set_var = set_var,
		.set_gamma = set_gamma,
		.blank = blank,
		.set_addr_win = fbtft_set_addr_win,
	},
};

```