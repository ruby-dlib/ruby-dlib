require 'dlib'

image_file = ARGV[0] || 'ext/dlib-19.4/examples/faces/2009_004587.jpg'

# you can download model file from http://dlib.net/files/mmod_human_face_detector.dat.bz2
model_file = 'mmod_human_face_detector.dat'
detector = Dlib::DNNFaceDetector.new(model_file)

puts 'single detections'

img = Dlib::Image.load(image_file)
rects = detector.detect(img)

puts "number of faces detected #{rects.length}"
rects.each.with_index do |rect, index|
  puts "Detection #{index}: Left: #{rect.left} Top: #{rect.top} Right: #{rect.right} Bottom: #{rect.bottom} Area: #{rect.area}"
end

puts ''
puts 'multiple detections'

image = Dlib::Image.load(image_file)
images = [image, image, image]
results = detector.detect(images)

results.each do |rects|
  puts "number of faces detected #{rects.length}"
  rects.each.with_index do |rect, index|
    puts "Detection #{index}: Left: #{rect.left} Top: #{rect.top} Right: #{rect.right} Bottom: #{rect.bottom} Area: #{rect.area}"
  end
end
