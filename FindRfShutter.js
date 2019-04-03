#!/usr/bin/env node
'use strict';
const fs = require('fs')
var command;
var out;
function dec2bin(dec){
     return (dec >>> 0).toString(2);
}

function getDateTime() {
    var date = new Date();
    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;
    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;
    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;
    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;
    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;
    return year + ":" + month + ":" + day + ":" + hour + ":" + min + ":" + sec;
}



function go () {
        var i;
        var preambolo ="110011000000";
        var direction="1001"; //up:1001 do:0010 st:0110
        var zero = "0000000000000000";
        var fine = "101000100000000000";
        var noti = ['0001100101010001','1001100100010001','0101100101100001','1101100100100001','0011100101000001','1011100100000001','0111100101111110','1111100100111110','0000010101011110'];
        for (i = 1; i < 65536; i++)
        {
         var id = dec2bin(-i).slice(-16);
         if  (!(noti.indexOf(id) > -1))
          { var tentativo = preambolo+direction+zero+id+fine;
            var command = '~/remote-control-shutters/send1.py '+ tentativo;
            var out = i + ' - ' + tentativo + ' - ' + getDateTime();            
            require('child_process').execSync(command, {stdio: 'inherit'} );                                      
            var logStream = fs.createWriteStream('log.txt', {'flags': 'a'});
            logStream.end(i+' - '+tentativo+'-'+getDateTime());
            console.log(out);
         }
      }
}
go();
