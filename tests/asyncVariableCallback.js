
var cBind = require('../build/Debug/cbind_core.node');

var asyncVarCb = cBind.asyncVariableCallback;

var mocha = require('mocha');

var chai = require('chai');
var assert = chai.assert;
chai.should();

var initialDate = (new Date()).getTime();


describe('asynchronous function set to variable', function(cb) {

  it('should return asynchronously after given time', function(cb) {

    asyncVarCb.cb = function() {
      assert( (new Date()).getTime() - initialDate >= 500);
      cb();
    };
    asyncVarCb.callbackAfterMs(500);

  });

});

