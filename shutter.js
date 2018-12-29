#!/usr/bin/env node
var http = require('http');
var url = require('url');
var exec = require('child_process').exec;

console.log("Http server starting...");

http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  var q = url.parse(req.url, true).query;
  console.log('=>',q);
  if ((q.shutter != undefined) && (q.shutter == 'upG')){
        var shutters = ['up6','up7','up8','up9'];
        shutters.forEach(function(s) {
                var command = '/home/pi/send.py '+ s;
                console.log('command',command);
                exec(command, function callback(error, stdout, stderr){
                        console.log('stdout: ' + stdout);
                        console.log('stderr: ' + stderr);
                        if (error !== null) {
                                console.log('exec error: ' + error);
                        }
                });
        res.write(s);
        });
  };
 if ((q.shutter != undefined) && (q.shutter != 'upG')) {
        var command = '/home/pi/send.py '+ q.shutter;
        console.log('command',command);
        exec(command, function callback(error, stdout, stderr){
                console.log('stdout: ' + stdout);
                console.log('stderr: ' + stderr);
                if (error !== null) {
                        console.log('exec error: ' + error);
                }
        });
        res.write(q.shutter);
}  
  
res.end();
}).listen(8889);
