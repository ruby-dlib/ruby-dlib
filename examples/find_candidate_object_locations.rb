require 'dlib'

image_file = ARGV[0] || 'ext/dlib-18.13/examples/faces/2009_004587.jpg'
img = Dlib::Image.load(image_file)

rects = Dlib.find_candidate_object_locations(img, min_size: 500)

puts "number of rectangles found #{rects.length}"
rects.each.with_index do |rect, index|
  puts "Detection #{index}: Left: #{rect.left} Top: #{rect.top} Right: #{rect.right} Bottom: #{rect.bottom}"
end
