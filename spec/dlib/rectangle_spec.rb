require 'spec_helper'

module Dlib
  describe Rectangle do
    def rectangle(*args)
      described_class.new(*args)
    end

    subject { rectangle(1, 2, 3, 4) }

    describe '#initialize' do
      context 'with left, top, right, and bottom positions' do
        it 'has correct values' do
          expect(subject.left).to   eq(1)
          expect(subject.top).to    eq(2)
          expect(subject.right).to  eq(3)
          expect(subject.bottom).to eq(4)
          expect(subject.width).to  eq(3)
          expect(subject.height).to eq(3)
        end
      end

      context 'with width and height' do
        subject { rectangle(2, 3) }

        it 'has correct values' do
          expect(subject.left).to   eq(0)
          expect(subject.top).to    eq(0)
          expect(subject.right).to  eq(1)
          expect(subject.bottom).to eq(2)
          expect(subject.width).to  eq(2)
          expect(subject.height).to eq(3)
        end
      end

      context 'without arguments' do
        subject { rectangle() }

        it 'has correct values' do
          expect(subject.left).to   eq(0)
          expect(subject.top).to    eq(0)
          expect(subject.right).to  eq(-1)
          expect(subject.bottom).to eq(-1)
          expect(subject.width).to  eq(0)
          expect(subject.height).to eq(0)
        end
      end
    end

    describe '#left=' do
      it 'set left value' do
        subject.left = 42
        expect(subject.left).to eq(42)
      end
    end

    describe '#top=' do
      it 'set top value' do
        subject.top = 42
        expect(subject.top).to eq(42)
      end
    end

    describe '#right=' do
      it 'set right value' do
        subject.right = 42
        expect(subject.right).to eq(42)
      end
    end

    describe '#bottom=' do
      it 'set bottom value' do
        subject.bottom = 42
        expect(subject.bottom).to eq(42)
      end
    end

    describe '#area' do
      it 'returns collect values' do
        expect(rectangle(1, 2, 3, 4).area).to eq(9)
        expect(rectangle(4, 4).area).to eq(16)
        expect(rectangle().area).to eq(0)
      end
    end

    describe '#empty?' do
      it 'returns collect values' do
        expect(rectangle(1, 2, 3, 4)).not_to be_empty
        expect(rectangle(4, 4)).not_to be_empty
        expect(rectangle()).to be_empty
      end

      specify 'There are different empty rectangles not to be equal' do
        r1 = rectangle(1, 1, 3, 3).intersect(rectangle())
        r2 = rectangle()
        expect(r1).not_to eq(r2)
      end
    end

    describe '#+' do
      it 'returns union of two rectangles' do
        expect(rectangle(1, 1, 3, 3) + rectangle(2, 2, 5, 5)).to eq(rectangle(1, 1, 5, 5))
        expect(rectangle(1, 2, 3, 5) + rectangle(2, 1, 5, 3)).to eq(rectangle(1, 1, 5, 5))
        expect(rectangle(1, 1, 3, 3) + rectangle()).to eq(rectangle(1, 1, 3, 3))
        expect(rectangle() + rectangle(1, 1, 3, 3)).to eq(rectangle(1, 1, 3, 3))
        expect(rectangle() + rectangle()).to eq(rectangle())
      end
    end

    describe '#intersect' do
      it 'returns intersection of two rectangles' do
        expect(rectangle(1, 1, 5, 5).intersect(rectangle(2, 2, 3, 3))).to eq(rectangle(2, 2, 3, 3))
        expect(rectangle(1, 1, 3, 3).intersect(rectangle(2, 2, 5, 5))).to eq(rectangle(2, 2, 3, 3))
        expect(rectangle(1, 1, 3, 3).intersect(rectangle())).to be_empty
        expect(rectangle().intersect(rectangle(1, 1, 3, 3))).to be_empty
        expect(rectangle().intersect(rectangle())).to be_empty
      end
    end
  end
end
