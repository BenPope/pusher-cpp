# Pusher client and server built on Boost.Asio

## Examples

The examples are a simple client and server designed to talk to each other.  They should work by just specifying your application credentials from the [Pusher Dashboard](https://dashboard.pusher.com/)

The server will send messages you type to the given `channel` and `event`; the client will receive them.  Sending `bye` will terminate both.

Running the examples without arguments will list the options.  Be sure to set the cluster if it is not `mt1`.

It may be possible to use the client to access existing public endpoints, but customisation is recommended.
