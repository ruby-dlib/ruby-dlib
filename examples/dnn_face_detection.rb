require 'dlib'

image_file = ARGV[0] || 'ext/dlib-19.2/examples/faces/2009_004587.jpg'

# you can download model file from http://dlib.net/files/mmod_human_face_detector.dat.bz2
model_file = 'mmod_human_face_detector.dat'
detector = Dlib::DNNFaceDetector.new(model_file)

img = Dlib::Image.load(image_file)
rects = detector.detect(img)

puts "number of faces detected #{rects.length}"
rects.each.with_index do |rect, index|
  puts "Detection #{index}: Left: #{rect.left} Top: #{rect.top} Right: #{rect.right} Bottom: #{rect.bottom} Area: #{rect.area}"
  img.draw_rectangle!(rect, [0, 0, 255]) if rect.area > 0
end

output_file = ARGV[1] || 'faces.jpg'
img.save_jpeg(output_file)
