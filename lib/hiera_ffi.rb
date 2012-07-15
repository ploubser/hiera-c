require 'ffi'

module HieraFFI
  extend FFI::Library
  ffi_lib "/Users/kelseyhightower/workspace/hiera-c/libhiera.so"
  attach_function :lookup_api, [:string, :string, :string, :string], :string
end

