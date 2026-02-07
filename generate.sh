mkdir obj
mkdir obj_pic
mkdir deps/
cd deps
mkdir -p include/gtma
mkdir lib
git clone https://github.com/mackron/dr_libs.git
git clone https://github.com/recp/cglm.git
git clone https://github.com/jkuhlmann/cgltf.git
git clone https://github.com/nothings/stb.git

# cGLM
cd cglm
cp -r include/cglm ../include
mkdir build
cd build
cmake ..
make
cp libcglm.so ../../lib/

# dr_libs
cd ../../dr_libs
cp dr_wav.h ../include

# cGLTF
cd ../cgltf
cp cgltf.h ../include

#stb
cd ../stb
cp stb_image.h ../include
