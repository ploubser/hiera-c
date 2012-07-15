module Puppet::Parser::Functions
  newfunction(:hierac, :type => :rvalue) do |*args|
    require 'hiera_ffi_puppet'
    key, default = HieraFFIPuppet.parse_args(args)
    HieraFFIPuppet.lookup(key, default, self.facts, 'priority')
  end
end

