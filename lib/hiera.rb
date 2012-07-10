require 'hiera_ffi'
require 'json'

key = 'port'
resolution_type   = 'priority'
hiera_config_file = '/etc/hiera.json'

scope = {
  'certname'        => 'agent.puppetlabs.com',
  'operatingsystem' => 'Debian',
  'environment'     => 'production',
}
scope_string = scope.to_json


answer = HieraFFI.lookup_api(key, resolution_type, scope_string, hiera_config_file)
puts answer
