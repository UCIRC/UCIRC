SAMPLES=" \
  CameraControl \
"

for SAMPLE in $SAMPLES; do
  make -C $SAMPLE
done

