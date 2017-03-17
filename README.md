# Dlib

Ruby binding of [dlib C++ library](http://dlib.net/).

## Installation

`dlib` depends libjpeg and libpng. So, you should install libraries at first.

**Mac**
```
$ brew install jpeg libpng
```

**Ubuntu 16.04**
```
$ apt-get install libjpeg8-dev libpng12-dev
```

**If you want to use DNN based face detector, you would have to install CUDA SDKs and cuDNN SDK.**
**Please read this page.** http://docs.nvidia.com/cuda/#axzz4anGdXQuB
**This gem try to use CUDA if /usr/local/cuda directory is exists.**

Add this line to your application's Gemfile:

```ruby
gem 'dlib'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install dlib

## Usage

See examples directory.
[https://github.com/ruby-dlib/ruby-dlib/tree/master/examples](https://github.com/ruby-dlib/ruby-dlib/tree/master/examples)

## Face Detector Comparison

|                   | CPU | GPU | recall rate | precision rate |
|-------------------|-----|-----|-------------|----------------|
| opencv haar based |  😄  |  -  |     😄       |       🤔        |
| dlib hog based *1 |  😄  |  -  |     😄       |       😻        |
| dlib dnn based *2 |  🤔  |  😄  |     😂       |       😻        |

1. http://blog.dlib.net/2014/02/dlib-186-released-make-your-own-object.html
2. http://blog.dlib.net/2016/10/easily-create-high-quality-object.html


demonstrated movie by original author of dlib on youtube


[![](https://img.youtube.com/vi/LsK0hzcEyHI/0.jpg)](http://www.youtube.com/watch?v=LsK0hzcEyHI "Click to play on Youtube.com")

## Contributing

1. Fork it ( https://github.com/ruby-dlib/ruby-dlib/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
