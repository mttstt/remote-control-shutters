#!/usr/bin/env node
//
// The input can be GOOGLE ASSISTANT + IFTTT
//
// Example input compatible:
// http://XXX.ddns.net:7000/?shutter=do6&up7&do8
// http://XXX.ddns.net:7000/?shutter=canc
// http://XXX.ddns.net:7000/?shutter=up0

var http = require('http');
var url = require('url');
var exec = require('child_process').exec;
var command;
console.log("Http server starting...");
http.createServer(function (req, res) {
  var command;
  res.writeHead(200, {'Content-Type': 'text/html'});
  var q = url.parse(req.url, true).query;
  console.log('shutter: ',q.shutter,' where: ',q.where);
  var qshutter = q.shutter;
  if ((q.where == 'up') || (q.where == 'do') || (q.where == 'st')) {
      switch (true){
        case /cameretta/.test(q.shutter):
                qshutter=q.where+"1";
                break;
        case /bimbe/.test(q.shutter):
                qshutter=q.where+"1";
                break;  
        case /nonna/.test(q.shutter):
                qshutter=q.where+"2";
                break;
        case /bagno grande/.test(q.shutter):
                qshutter=q.where+"3";
                break;
        case /camera matrimoniale/.test(q.shutter):
                qshutter=q.where+"4";
                break;
        case (/bagno/.test(q.shutter) && /piccolo/.test(q.shutter) ):
                qshutter=q.where+"5";
                break;
        case (/sala/.test(q.shutter) && /piccola/.test(q.shutter) ):
                qshutter=q.where+"6";
                break;
        case (/sala/.test(q.shutter) && /grande/.test(q.shutter) ):
                qshutter=q.where+"7";
                break;
        case /ripostiglio/.test(q.shutter):
                qshutter=q.where+"8";
                break;
        case /stanzino/.test(q.shutter):
                qshutter=q.where+"8";
                break; 
        case /cucina/.test(q.shutter):
                qshutter=q.where+"9";
                break;
        case /sala/.test(q.shutter):
                qshutter=q.where+"6 "+q.where+"7";
                break;
        case (/zona/.test(q.shutter) && /giorno/.test(q.shutter)):
                qshutter=q.where+"6 "+q.where+"7 "+q.where+"8 "+q.where+"9";
                break;
        default:
                console.log("Error in input: "+q.shutter+" => "+(q.shutter.split(" ").splice(-1)));
      }
   }
   if (qshutter === null){ qshutter=q.where; }
   command = '~/remote-control-shutters/send.py '+ qshutter;
   console.log('command: ',command);
   exec(command, function callback(error, stdout, stderr){
     console.log('stdout: ' + stdout);
     console.log('stderr: ' + stderr);
     if (error !== null) {console.log('exec error: ' + error); }
   });
   res.write(command);
   res.end();
}).listen(7000);
