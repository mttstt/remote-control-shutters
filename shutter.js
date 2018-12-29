#!/usr/bin/env node

var http = require('http');
var url = require('url');
var exec = require('child_process').exec;

console.log("Http server starting...");

http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  var q = url.parse(req.url, true).query;
  console.log('=>',q);

  if (q.shutter != undefined) {
        var command = '/home/pi/send.py '+ q.shutter;
        console.log('command',command);
        exec(command, function callback(error, stdout, stderr){
                console.log('stdout: ' + stdout);
                console.log('stderr: ' + stderr);
                if (error !== null) {
                        console.log('exec error: ' + error);
                }
        });
        res.write(command);
  }
 res.end();
}).listen(8889);
