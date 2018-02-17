#!/usr/bin/env node

var http = require('http');
var url = require('url');
var exec = require('child_process').exec;


http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  var q = url.parse(req.url, true).query;
  var command = '/home/pi/send.py '+ q.shutter;
  exec(command, function callback(error, stdout, stderr){
        console.log(stdout);
  });

  res.write(command);
  res.end();
}).listen(8888);

