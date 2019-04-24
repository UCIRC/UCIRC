SAMPLES=" \
  CameraControl \
  Config		\
"

for SAMPLE in $SAMPLES; do
  make -C $SAMPLE
done

