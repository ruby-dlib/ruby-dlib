require 'dlib'

image_file = ARGV[0] || 'ext/dlib-19.4/examples/faces/2009_004587.jpg'
img = Dlib::Image.load(image_file)

rects = Dlib.find_candidate_object_locations(img, min_size: 500)

puts "number of rectangles found #{rects.length}"
rects.each.with_index do |rect, index|
  puts "Detection #{index}: Left: #{rect.left} Top: #{rect.top} Right: #{rect.right} Bottom: #{rect.bottom} Area: #{rect.area}"
  img.draw_rectangle!(rect, [0, 0, 255]) if rect.area > 0
end

output_file = ARGV[1] || 'marked.jpg'
img.save_jpeg(output_file)
