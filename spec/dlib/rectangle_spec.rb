require 'spec_helper'

module Dlib
  describe Rectangle do
    describe '#initialize' do
      context 'with left, top, right, and bottom positions' do
        subject { described_class.new(1, 2, 3, 4) }

        it 'has correct values' do
          expect(subject.left).to   eq(1)
          expect(subject.top).to    eq(2)
          expect(subject.right).to  eq(3)
          expect(subject.bottom).to eq(4)
        end
      end

      context 'with width and height' do
        subject { described_class.new(2, 3) }

        it 'has correct values' do
          expect(subject.left).to   eq(0)
          expect(subject.top).to    eq(0)
          expect(subject.right).to  eq(1)
          expect(subject.bottom).to eq(2)
        end
      end

      context 'without arguments' do
        subject { described_class.new() }

        it 'has correct values' do
          expect(subject.left).to   eq(0)
          expect(subject.top).to    eq(0)
          expect(subject.right).to  eq(-1)
          expect(subject.bottom).to eq(-1)
        end
      end
    end
  end
end
