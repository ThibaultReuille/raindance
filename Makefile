
all:
	@echo "--- Packing raindance resources ---"
	mkdir -p Pack

	mkdir -p Pack/Shaders

	xxd -i Resources/Shaders/logo.vert > Pack/Shaders/logo.vert
	xxd -i Resources/Shaders/logo.frag > Pack/Shaders/logo.frag

	xxd -i Resources/Shaders/axis.vert > Pack/Shaders/axis.vert
	xxd -i Resources/Shaders/axis.frag > Pack/Shaders/axis.frag

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

	xxd -i Resources/Shaders/isovolume.vert > Pack/Shaders/isovolume.vert
	xxd -i Resources/Shaders/isovolume.frag > Pack/Shaders/isovolume.frag

	mkdir -p Pack/Shaders/Primitives

	xxd -i Resources/Shaders/Primitives/box.vert > Pack/Shaders/Primitives/box.vert
	xxd -i Resources/Shaders/Primitives/box.frag > Pack/Shaders/Primitives/box.frag

	xxd -i Resources/Shaders/Primitives/grid.vert > Pack/Shaders/Primitives/grid.vert
	xxd -i Resources/Shaders/Primitives/grid.frag > Pack/Shaders/Primitives/grid.frag

	mkdir -p Pack/Shaders/Charts

	xxd -i Resources/Shaders/Charts/LineChartBackground.vert > Pack/Shaders/Charts/LineChartBackground.vert
	xxd -i Resources/Shaders/Charts/LineChartBackground.frag > Pack/Shaders/Charts/LineChartBackground.frag
	xxd -i Resources/Shaders/Charts/LineChartGraph.vert > Pack/Shaders/Charts/LineChartGraph.vert
	xxd -i Resources/Shaders/Charts/LineChartGraph.frag > Pack/Shaders/Charts/LineChartGraph.frag

	xxd -i Resources/Shaders/Charts/HeightMap.vert > Pack/Shaders/Charts/HeightMap.vert
	xxd -i Resources/Shaders/Charts/HeightMap.frag > Pack/Shaders/Charts/HeightMap.frag

	mkdir -p Pack/Textures

	xxd -i Resources/Textures/umbrella-logo.png > Pack/Textures/umbrella-logo.png
	xxd -i Resources/Textures/script.png > Pack/Textures/script.png
	xxd -i Resources/Textures/mark.png > Pack/Textures/mark.png
	xxd -i Resources/Textures/chart-disk.png > Pack/Textures/chart-disk.png

	mkdir -p Pack/Textures/Shapes

	xxd -i Resources/Textures/Shapes/cloud.png > Pack/Textures/Shapes/cloud.png
	xxd -i Resources/Textures/Shapes/cross.png > Pack/Textures/Shapes/cross.png
	xxd -i Resources/Textures/Shapes/disk.png > Pack/Textures/Shapes/disk.png
	xxd -i Resources/Textures/Shapes/heart.png > Pack/Textures/Shapes/heart.png
	xxd -i Resources/Textures/Shapes/hexagon.png > Pack/Textures/Shapes/hexagon.png
	xxd -i Resources/Textures/Shapes/house.png > Pack/Textures/Shapes/house.png
	xxd -i Resources/Textures/Shapes/losange.png > Pack/Textures/Shapes/losange.png
	xxd -i Resources/Textures/Shapes/octagon.png > Pack/Textures/Shapes/octagon.png
	xxd -i Resources/Textures/Shapes/patch.png > Pack/Textures/Shapes/patch.png
	xxd -i Resources/Textures/Shapes/pentagon.png > Pack/Textures/Shapes/pentagon.png
	xxd -i Resources/Textures/Shapes/semicircle.png > Pack/Textures/Shapes/semicircle.png
	xxd -i Resources/Textures/Shapes/square.png > Pack/Textures/Shapes/square.png
	xxd -i Resources/Textures/Shapes/star.png > Pack/Textures/Shapes/star.png
	xxd -i Resources/Textures/Shapes/triangle.png > Pack/Textures/Shapes/triangle.png
	xxd -i Resources/Textures/Shapes/triangle1.png > Pack/Textures/Shapes/triangle1.png
	xxd -i Resources/Textures/Shapes/triangle2.png > Pack/Textures/Shapes/triangle2.png

clean:
	rm -rf Pack
	rm -rf *~
	rm -rf */*~


