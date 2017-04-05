require 'spec_helper'

describe Dlib::Image do
  describe '.pyramid_up!' do
    context 'invalid argument' do
      it 'should raise TypeError' do
        expect { Dlib::Image.pyramid_up!(Object.new) }.to raise_error(TypeError)
      end
    end

    context 'valid argument' do
      it 'should scale up to 2 times' do
        file = fixture_file('face.jpg')
        image = Dlib::Image.load(file.path)

        expect(image.nr).to be(500)
        expect(image.nc).to be(400)

        expect { Dlib::Image.pyramid_up!(image) }.not_to raise_error

        expect(image.nr).to be(1001)
        expect(image.nc).to be(802)
      end
    end
  end
end
