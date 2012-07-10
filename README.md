# C Hiera

Hiera rewritten in ANSI C!

**Note:** Do not use!!

## Installation

    $ git clone git://github.com/kelseyhightower/hiera-c.git 
    $ cd hiera-c
    $ make
    $ make install

    $ gem install hiera-ffi
    $ gem install hiera-ffi-puppet

## Configuration

C Hiera uses a JSON formatted configuration file:

/etc/hiera.json

```json
{
  "hierarchy": [
    "%{certname}",
    "%{environment}",
    "%{operatingsystem}",
    "default"
  ],
  "datadir": "/var/lib/hiera/json"
}
```

JSON was chosen over YAML for simplicity and consistency with the JSON
backend.

Unlike Hiera, Hiera-C does not support pluggable backends; there is only a
JSON backend. So there is no `backend` key in the hiera config file.

## Usage

    $ ./hiera --help
    Usage: hiera [options] key

      --help    Show usage info.
      --config  The Hiera configuration file to use.
      --array   Array search.
      --scope   The scope JSON file to use.

## Examples

### Setup

#### Configuration:

/etc/hiera.json

```json
{
  "hierarchy": [
    "%{certname}",
    "%{environment}",
    "%{operatingsystem}",
    "default"
  ],
  "data_directory": "/var/lib/hiera"
}
```

#### Data:

/var/lib/hiera/agent.puppetlabs.com.json

```json
{
  "nginx::port": 80,
  "classes": [
    "nginx",
    "ssh::server"
  ]
}
```

-

/var/lib/hiera/production.json

```json
{
  "port": 443,
  "classes": [
    "production"
  ]
  "ntpservers": [
    "production.time.puppetlabs.com"
  ]
}
```

-

/var/lib/hiera/default.json

```json
{
  "port": 80,
  "classes": [
    "mco",
    "ntp"
  ]
  "ntpservers": [
    "default.time.puppetlabs.com"
  ]
}
```

#### Scope:

/tmp/scope.json

```json
{
  "certname": "agent.puppetlabs.com",
  "environment": "production",
}
```

### CLI

    $ ./hiera port --scope=/tmp/scope.json

