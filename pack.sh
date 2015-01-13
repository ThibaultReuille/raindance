#!/bin/sh

rm -rf Pack.hh
echo "/*" >> Pack.hh
echo " * --- Packed Resources ---" >> Pack.hh
echo " */" >> Pack.hh
echo "#pragma once" >> Pack.hh

echo "- Packing Shaders"

xxd -i Assets/Shaders/logo.vert >> Pack.hh
xxd -i Assets/Shaders/logo.frag >> Pack.hh

xxd -i Assets/Shaders/ray.vert >> Pack.hh
xxd -i Assets/Shaders/ray.frag >> Pack.hh

xxd -i Assets/Shaders/pointcloud.vert >> Pack.hh
xxd -i Assets/Shaders/pointcloud.frag >> Pack.hh

xxd -i Assets/Shaders/convolution.vert >> Pack.hh
xxd -i Assets/Shaders/convolution.frag >> Pack.hh

xxd -i Assets/Shaders/wallpaper.vert >> Pack.hh
xxd -i Assets/Shaders/wallpaper.frag >> Pack.hh

xxd -i Assets/Shaders/icon.vert >> Pack.hh
xxd -i Assets/Shaders/icon.frag >> Pack.hh

xxd -i Assets/Shaders/text.vert >> Pack.hh
xxd -i Assets/Shaders/text.frag >> Pack.hh

xxd -i Assets/Shaders/environment.vert >> Pack.hh
xxd -i Assets/Shaders/environment.frag >> Pack.hh

xxd -i Assets/Shaders/isovolume.vert >> Pack.hh
xxd -i Assets/Shaders/isovolume.frag >> Pack.hh

echo "- Packing Shaders/Primitives"

xxd -i Assets/Shaders/Primitives/axis.vert >> Pack.hh
xxd -i Assets/Shaders/Primitives/axis.frag >> Pack.hh

xxd -i Assets/Shaders/Primitives/box.vert >> Pack.hh
xxd -i Assets/Shaders/Primitives/box.frag >> Pack.hh

xxd -i Assets/Shaders/Primitives/grid.vert >> Pack.hh
xxd -i Assets/Shaders/Primitives/grid.frag >> Pack.hh

xxd -i Assets/Shaders/Primitives/wideline.vert >> Pack.hh
xxd -i Assets/Shaders/Primitives/wideline.frag >> Pack.hh

xxd -i Assets/Shaders/Primitives/polyline.vert >> Pack.hh
xxd -i Assets/Shaders/Primitives/polyline.frag >> Pack.hh

echo "- Packing Shaders/Widgets"

xxd -i Assets/Shaders/Widgets/widget.vert >> Pack.hh
xxd -i Assets/Shaders/Widgets/widget.frag >> Pack.hh

xxd -i Assets/Shaders/Widgets/slider.vert >> Pack.hh
xxd -i Assets/Shaders/Widgets/slider.frag >> Pack.hh

xxd -i Assets/Shaders/Widgets/checkbox.vert >> Pack.hh
xxd -i Assets/Shaders/Widgets/checkbox.frag >> Pack.hh

xxd -i Assets/Shaders/Widgets/timeline.vert >> Pack.hh
xxd -i Assets/Shaders/Widgets/timeline.frag >> Pack.hh

echo "- Packing Shaders/Charts"

xxd -i Assets/Shaders/Charts/LineChartBackground.vert >> Pack.hh
xxd -i Assets/Shaders/Charts/LineChartBackground.frag >> Pack.hh
xxd -i Assets/Shaders/Charts/LineChartGraph.vert >> Pack.hh
xxd -i Assets/Shaders/Charts/LineChartGraph.frag >> Pack.hh

xxd -i Assets/Shaders/Charts/HeightMap.vert >> Pack.hh
xxd -i Assets/Shaders/Charts/HeightMap.frag >> Pack.hh

echo "- Packing Textures"

xxd -i Assets/Textures/umbrella-logo.png >> Pack.hh
xxd -i Assets/Textures/script.png >> Pack.hh
xxd -i Assets/Textures/mark.png >> Pack.hh
xxd -i Assets/Textures/chart-disk.png >> Pack.hh
xxd -i Assets/Textures/clock.png >> Pack.hh

echo "- Packing Textures/Shapes"

xxd -i Assets/Textures/Shapes/cloud.png >> Pack.hh
xxd -i Assets/Textures/Shapes/cross.png >> Pack.hh
xxd -i Assets/Textures/Shapes/disk.png >> Pack.hh
xxd -i Assets/Textures/Shapes/heart.png >> Pack.hh
xxd -i Assets/Textures/Shapes/hexagon.png >> Pack.hh
xxd -i Assets/Textures/Shapes/house.png >> Pack.hh
xxd -i Assets/Textures/Shapes/losange.png >> Pack.hh
xxd -i Assets/Textures/Shapes/octagon.png >> Pack.hh
xxd -i Assets/Textures/Shapes/patch.png >> Pack.hh
xxd -i Assets/Textures/Shapes/pentagon.png >> Pack.hh
xxd -i Assets/Textures/Shapes/semicircle.png >> Pack.hh
xxd -i Assets/Textures/Shapes/square.png >> Pack.hh
xxd -i Assets/Textures/Shapes/star.png >> Pack.hh
xxd -i Assets/Textures/Shapes/triangle.png >> Pack.hh
xxd -i Assets/Textures/Shapes/triangle1.png >> Pack.hh
xxd -i Assets/Textures/Shapes/triangle2.png >> Pack.hh
xxd -i Assets/Textures/Shapes/forbidden.png >> Pack.hh


