# Mesh Segmentation

Running:
``` bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

In the original implementation, labels are stored at face corners. In our implementation, the vertices are split according to labels in default. If you want to use the original implementation, set `per_face_corner_color=true` in the ply writer.