#!/usr/bin/env node
#
# Example input command:
# http://matteosetti.dlinkddns.com:8889/?shutter=do6&up7&do8

var http = require('http');
var url = require('url');
var exec = require('child_process').exec;

console.log("Http server starting...");

http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});

  var q = url.parse(req.url, true).query;

 console.log('shutter: ',q);


 if (q.shutter != undefined) {
  var qshutter = q.shutter;

  console.log('shutter: ',q.shutter,' where: ',q.where);

  if ((q.where == 'up') || (q.where == 'do') || (q.where == 'st')) {
      var id;
      switch (true){
        case /cameretta/.test(q.shutter):
                id="1";
                break;
        case (/camera/.test(q.shutter) && /nonna/.test(q.shutter) ):
                id="2";
                break;
        case /bagno grande/.test(q.shutter):
                id="3";
                break;
        case /camera matrimoniale/.test(q.shutter):
                id="4";
                break;
        case (/bagno/.test(q.shutter) && /piccolo/.test(q.shutter) ):
                id="5";
                break;
        case (/sala/.test(q.shutter) && /piccola/.test(q.shutter) ):
                id="6";
                break;
        case (/sala/.test(q.shutter) && /grande/.test(q.shutter) ):
                id="7";
                break;
        case /ripostiglio/.test(q.shutter):
                id="8";
                break;
        case /cucina/.test(q.shutter):
                id="9"
                break;
        case /sala/.test(q.shutter):
                id="6&7"
                break;
        default:
                console.log("Error in input: "+q.shutter+" => "+(q.shutter.split(" ").splice(-1)));
        }

        qshutter=q.where+id;
        if (id == "6&7") {qshutter = q.where+"6 "+q.where+"7" }
   }

   console.log('command: ',qshutter);
   var command = '/home/pi/send.py '+ qshutter;
   //console.log('command',command);
   exec(command, function callback(error, stdout, stderr){
     console.log('stdout: ' + stdout);
     console.log('stderr: ' + stderr);
        if (error !== null) {
                console.log('exec error: ' + error);
        }
   });
 }
 res.write(command);
 res.end();
}).listen(7000);


