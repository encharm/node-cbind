
var cBind = require('../build/Debug/cbind_core.node');

var functions = cBind.functions;

var mocha = require('mocha');

var chai = require('chai');
var assert = chai.assert;
chai.should();

var typeofObject = require('../testUtils').typeofObject;

var counter = function() {
  functions.getCounter().should.equal(functions.callCounter);
  return functions.getCounter();
};


describe('native module', function() {
  it('should export proper API', function() {
    assert.deepEqual(typeofObject(functions), {
      callCounter: 'number',
      doubleFunction: 'function',
      floatFunction: 'function',
      intFunction: 'function',
      voidFunction: 'function',
      cStringFunction: 'function',
      stdStringFunction: 'function',
      functionTakingInt: 'function',
      functionTakingFloat: 'function',
      functionTakingDouble: 'function',
      functionTakingCString: 'function',
      functionTakingStdString: 'function',
      functionConcat: 'function',
      functionConcat2: 'function',
      functionConcat3: 'function',
      functionConcat4: 'function',
      getCounter: 'function',
      resetTestData: 'function',
      someNamespace: 'object'
    }, "expected cbind API");
  });
});

describe('testing procedures', function() {

  it('should work', function() {
    counter().should.equal(0);
    functions.callCounter = 1;
    counter().should.equal(1);
    functions.callCounter = 0;
    counter().should.equal(0);
  });

});

describe('basic functions support', function() {

  beforeEach(function() {
    functions.callCounter = 0;
  });

  afterEach(function() {
    counter().should.equal(1);
  });

  it('should call void()', function() {
    functions.voidFunction();
  });
  it('should call int()', function() {
    functions.intFunction().should.equal(42);
  });

  it('should call float()', function() {
    functions.floatFunction().should.equal(42);
  });

  it('should call double()', function() {
    functions.doubleFunction().should.equal(42);
  });

  it('should call const char*()', function() {
    functions.cStringFunction().should.equal('hello');
  });

  it('should call std::string()', function() {
    functions.stdStringFunction().should.equal('hello');
  });

  it('should call int(int)', function() {
    functions.functionTakingInt(42).should.equal(42);
  });

  it('should call float(float)', function() {
    functions.functionTakingFloat(42).should.equal(42);
  });

  it('should call double(double)', function() {
    functions.functionTakingDouble(42).should.equal(42);
  });

  it('should call const char*(const char*)', function() {
    functions.functionTakingCString('42').should.equal('42');
  });

  it('should call std::string(std::string)', function() {
    functions.functionTakingStdString('42').should.equal('42');
  });

  it('should throw exception when argument is of bad type', function() {
    try {
      functions.functionTakingStdString(42)
      assert(false);
    } catch(err) {
      functions.callCounter++;
      err.message.should.equal('Cannot convert argument 1 to type: std::string')
    }
  });

  it('should call function std::string(int, float, double, const char*, std::string)', function() {
    var args = [124, 484, 213, 'hello', 'world'];
    functions.functionConcat.apply(null, args).should.equal(args.join(','));
  });

  it('should call function char*(int, float, double, const char*, std::string) that automatically frees result after conversion to v8', function() {
    var args = [13424, 912, 324, 'foo', 'bar'];
    functions.functionConcat2.apply(null, args).should.equal(args.join(','));
  });

  it('should call function char*(int, float, double, const char*, std::string) that automatically frees result after conversion to v8', function() {
    var args = [9219, 35, 4444, 'eenie', 'meenie'];
    functions.functionConcat3.apply(null, args).should.equal(args.join(','));
  });

  // not working yet
  it('should call function char*(int, float, double, const char*, std::string) that automatically frees result after conversion to v8', function() {
    var args = [9219, 35, 4444, 'eenie', 'meenie'];
    var expected = args.join(',');
    assert.deepEqual(functions.functionConcat4.apply(null, args),
    {
      out: expected,
      retValue: expected.length
    });
  });

  it('should call function in namespace', function() {
    functions.someNamespace.functionInNamespace().should.equal(99112331);
  });

});