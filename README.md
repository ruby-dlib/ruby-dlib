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

**If you want to use DNN based face detector, you would have to install CUDA SDKs.**
**Please read this page.** http://docs.nvidia.com/cuda/#axzz4anGdXQuB

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

## Contributing

1. Fork it ( https://github.com/mrkn/ruby-dlib/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
