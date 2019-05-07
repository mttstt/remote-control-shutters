#!/usr/bin/env node

// Copyright 2016, mtt stt, Inc.
//
// Accept HTTPS and HTTP calls.
// HTTPS POST calls 
//   are used in conjunction with "Actions on Google" Node.js client library for temperature and humidity in rooms
// 
// HTTP GET calls 
//
// => NEW con IFTT
// http://xxx.dlinkddns.com:7000/?shutter={{TextField}}&where=st
//
// => OLD
//   are used in conjuction with IFTTT for move shutters and gate
//   Here some examples oh http GET calls:
//     http://XXX.dlinkddns.com:7000/?shutter=do6&up7&do8
//     http://XXX.dlinkddns.com:7000/?shutter=canc
//     http://XXX.dlinkddns.com:7000/?shutter=up0

'use strict';
const express = require('express');
const http = require('http');
const https = require('https')
const bodyParser = require('body-parser');
const {dialogflow} = require('actions-on-google');
const app = dialogflow({debug: true});
const fs = require('fs')
const expressApp  = express().use(bodyParser.json());
const expressApps = express().use(bodyParser.json());
var url = require('url');
var exec = require('child_process').exec;
var command;

const rooms = { "corridoio":"192.168.1.111", "sala": "192.168.1.112", "cucina": "192.168.1.113", "camera matrimoniale": "192.168.1.114"};

expressApps.get('/', (req, res) => {
  res.send('Hello HTTPS!')
})

//expressApp.get('/', (req, res) => {
//  res.send('Hello HTTP!')
//})
//http.createServer(expressApp).listen(7000, () => {
//  console.log('Listening...7000')
//})

expressApps.post('/fulfillment', app);


function temperatura(stanza) {
   return new Promise(((resolve) => {
        var ip = rooms[stanza];
        http.get("http://"+ip+"/json", (res) => {
          const { statusCode } = res;
          const contentType = res.headers['content-type'];
          let error;
          if (statusCode !== 200) {
            error = new Error('Request Failed.\n' +
                              `Status Code: ${statusCode}`);
          } else if (!/^application\/json/.test(contentType)) {
            error = new Error('Invalid content-type.\n' + `Expected application/json but received ${contentType}`);
          }
          if (error) {
            console.error(error.message);
            res.resume();
            return;
          }
          res.setEncoding('utf8');
          let rawData = '';
          res.on('data', (chunk) => { rawData += chunk; });
          res.on('end', () => {
            try {
              const parsedData = JSON.parse(rawData);
              var data = [parsedData.Sensors[0].Temperature, parsedData.Sensors[0].Humidity];
              console.log('====> ',data);
              resolve(data);
            } catch (e) {
              console.error(e.message);
            }
          });
          }).on('error', (e) => { console.error(`Got error: ${e.message}`);});
  }));
}

app.intent('Require temperature in the room', (conv,{stanza}) => {
  return temperatura(stanza).then((data) => {
    conv.close(`La tempertura in ${stanza} è pari a ${data[0]} gradi centigradi e l'umidità è pari al ${data[1]} percento.`);
  })
})


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
}).listen(7000, () => {
  console.log('Listening...7000')
})

https.createServer({
  key: fs.readFileSync('/home/pi/~/.certbot/config/live/smatteo.dlinkddns.com/privkey.pem'),
  cert: fs.readFileSync('/home/pi/~/.certbot/config/live/smatteo.dlinkddns.com/cert.pem'),
  ca: fs.readFileSync('/home/pi/~/.certbot/config/live/smatteo.dlinkddns.com/chain.pem')
  }, expressApps).listen(7443, () => {
 console.log('Listening...7443')
})

          
          
