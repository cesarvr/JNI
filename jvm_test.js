var jvm = require('./build/Release/jvm.node');
var http = require('http');


var options = {
    host: 'www.gnu.org',
    //path: 'wiki/B2FH_paper'
};
jvm.start();
var str = '';
var callback = function(response) {

    //another chunk of data has been recieved, so append it to `str`
    response.on('data', function(chunk) {
        str += chunk;
    });

    //the whole response has been recieved, so we just print it out here
    response.on('end', function() {
        console.log(str);
        jvm.create_pdf(str);
    });
}

http.request(options, callback).end();
