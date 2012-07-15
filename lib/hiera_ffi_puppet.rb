require 'json'
require 'puppet'
require 'hiera_ffi'

module HieraFFIPuppet
  module_function

  def lookup(key, default, scope, resolution_type)
    hiera_config_file = '/etc/hiera.json'
    scope_string = scope.to_json
    answer = HieraFFI.lookup_api(key, resolution_type, scope_string, hiera_config_file)
    answer = JSON.load(answer)

    return answer[key]
  end

  def parse_args(args)
    # Functions called from Puppet manifests like this:
    #
    #   hiera("foo", "bar")
    #
    # Are invoked internally after combining the positional arguments into a
    # single array:
    #
    #   func = function_hiera
    #   func(["foo", "bar"])
    #
    # Functions called from templates preserve the positional arguments:
    #
    #   scope.function_hiera("foo", "bar")
    #
    # Deal with Puppet's special calling mechanism here.
    if args[0].is_a?(Array)
      args = args[0]
    end

    if args.empty?
      raise(Puppet::ParseError, "Please supply a parameter to perform a Hiera lookup")
    end

    key      = args[0]
    default  = args[1]

    return [key, default]
  end
end

