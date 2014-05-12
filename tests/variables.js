var tov8 = require('../build/Debug/tov8_core.node');

var variables = tov8.variables;

var mocha = require('mocha');

var chai = require('chai');
var assert = chai.assert;
chai.should();


describe('basic global variables support', function() {

  it('should properly get default values', function() {
    assert.deepEqual({
      globalInt: variables.globalInt,
      globalFloat: variables.globalFloat,
      globalDouble: variables.globalDouble,
      globalCString: variables.globalCString,
      globalStdString: variables.globalStdString,
      globalPointerToInt: variables.globalPointerToInt,
      globalPointerToFloat: variables.globalPointerToFloat,
      globalPointerToDouble: variables.globalPointerToDouble
    }, {
      globalInt: 0,
      globalFloat: 0,
      globalDouble: 0,
      globalCString: null,
      globalStdString: '',
      globalPointerToInt: null,
      globalPointerToFloat: null,
      globalPointerToDouble: null
    });

    variables.verifyGlobalInt42().should.equal(false);

  });

  it('should set new value for int', function() {
    variables.globalInt = 42;
    variables.globalInt.should.equal(42);
    variables.verifyGlobalInt42().should.equal(true);
  });

  it('should throw exception if setting int from not a number', function() {
    try {
      variables.globalInt = 'foo';
      assert(false);
    } catch(err) {
      err.message.should.equal('Cannot convert value to type: int');
    }
  });

  it('should set new value for float', function() {
    variables.globalFloat = 43;
    variables.globalFloat.should.equal(43);
    variables.verifyGlobalFloat43().should.equal(true);
  });

  it('should throw exception if setting float from not a number', function() {
    try {
      variables.globalFloat = 'foo';
      assert(false);
    } catch(err) {
      err.message.should.equal('Cannot convert value to type: float');
    }
  });

  it('should set new value for double', function() {
    variables.globalDouble = 44;
    variables.globalDouble.should.equal(44);
    variables.verifyGlobalDouble44().should.equal(true);
  });

  it('should throw exception if setting double from not a number', function() {
    try {
      variables.globalDouble = 'foo';
      assert(false);
    } catch(err) {
      err.message.should.equal('Cannot convert value to type: double');
    }
  });

  it('should set new value for C-string', function() {
    variables.globalCString = '45';
    variables.globalCString.should.equal('45');
    variables.verifyGlobalCString45().should.equal(true);
  });

  it('should throw exception if setting C-String from not a string', function() {
    try {
      variables.globalCString = 42;
      assert(false);
    } catch(err) {
      err.message.should.equal('Cannot convert value to type: const char*');
    }
  });

  it('should set new value for std::string', function() {
    variables.globalStdString = '46';
    variables.globalStdString.should.equal('46');
    variables.verifyGlobalStdString46().should.equal(true);
  });

  it('should throw exception if setting std::String from not a string', function() {
    try {
      variables.globalStdString = 42;
      assert(false);
    } catch(err) {
      err.message.should.equal('Cannot convert value to type: std::string');
    }
  });

  it('should set int* from number', function() {
    variables.globalPointerToInt = tov8.createInt(42);
    tov8.derefInt(variables.globalPointerToInt).should.equal(42);
    variables.verifyPointerToInt42().should.equal(true);
  });

  it('should set float* from number', function() {
    variables.globalPointerToFloat = tov8.createFloat(43);
    tov8.derefFloat(variables.globalPointerToFloat).should.equal(43);
    variables.verifyPointerToFloat43().should.equal(true);
  });

  it('should set double* from number', function() {
    variables.globalPointerToDouble = tov8.createDouble(44);
    tov8.derefDouble(variables.globalPointerToDouble).should.equal(44);
    variables.verifyPointerToDouble44().should.equal(true);
  });

  it('should fail to dereference float type from double type', function() {

    try {
      tov8.derefFloat(variables.globalPointerToDouble).should.equal(44);
    } catch(err) {
      err.message.should.equal("Unable to unbox pointer float* from pointer of type double*");
    }

  });

});