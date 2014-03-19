
all:
	@echo "--- Packing raindance resources ---"
	mkdir -p Pack

	mkdir -p Pack/Shaders

	xxd -i Resources/Shaders/logo.vert > Pack/Shaders/logo.vert
	xxd -i Resources/Shaders/logo.frag > Pack/Shaders/logo.frag

	xxd -i Resources/Shaders/axis.vert > Pack/Shaders/axis.vert
	xxd -i Resources/Shaders/axis.frag > Pack/Shaders/axis.frag

	xxd -i Resources/Shaders/box.vert > Pack/Shaders/box.vert
	xxd -i Resources/Shaders/box.frag > Pack/Shaders/box.frag

	xxd -i Resources/Shaders/ray.vert > Pack/Shaders/ray.vert
	xxd -i Resources/Shaders/ray.frag > Pack/Shaders/ray.frag

	xxd -i Resources/Shaders/pointcloud.vert > Pack/Shaders/pointcloud.vert
	xxd -i Resources/Shaders/pointcloud.frag > Pack/Shaders/pointcloud.frag

	xxd -i Resources/Shaders/convolution.vert > Pack/Shaders/convolution.vert
	xxd -i Resources/Shaders/convolution.frag > Pack/Shaders/convolution.frag

	xxd -i Resources/Shaders/wallpaper.vert > Pack/Shaders/wallpaper.vert
	xxd -i Resources/Shaders/wallpaper.frag > Pack/Shaders/wallpaper.frag

	xxd -i Resources/Shaders/icon.vert > Pack/Shaders/icon.vert
	xxd -i Resources/Shaders/icon.frag > Pack/Shaders/icon.frag

	xxd -i Resources/Shaders/text.vert > Pack/Shaders/text.vert
	xxd -i Resources/Shaders/text.frag > Pack/Shaders/text.frag

	xxd -i Resources/Shaders/widget.vert > Pack/Shaders/widget.vert
	xxd -i Resources/Shaders/widget.frag > Pack/Shaders/widget.frag

	xxd -i Resources/Shaders/slider.vert > Pack/Shaders/slider.vert
	xxd -i Resources/Shaders/slider.frag > Pack/Shaders/slider.frag

	xxd -i Resources/Shaders/checkbox.vert > Pack/Shaders/checkbox.vert
	xxd -i Resources/Shaders/checkbox.frag > Pack/Shaders/checkbox.frag

	xxd -i Resources/Shaders/environment.vert > Pack/Shaders/environment.vert
	xxd -i Resources/Shaders/environment.frag > Pack/Shaders/environment.frag

	xxd -i Resources/Shaders/isovolume.vert > Pack/Shaders/isovolume.vert
	xxd -i Resources/Shaders/isovolume.frag > Pack/Shaders/isovolume.frag

	mkdir -p Pack/Textures

	xxd -i Resources/Textures/umbrella-logo.png > Pack/Textures/umbrella-logo.png
	xxd -i Resources/Textures/script.png > Pack/Textures/script.png
	xxd -i Resources/Textures/mark.png > Pack/Textures/mark.png

clean:
	rm -rf Pack
	rm -rf *~
	rm -rf */*~

