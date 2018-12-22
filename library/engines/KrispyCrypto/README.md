# What is KrispyCrypto
KrispyCrypto is RadJav's interface to underlaying cryptographic libraries. Currently only OpenSSL v 1.1.1 is supported. However, the interface allows for using other libraries as well. When more libraries are added, developers will be able to pick, and mix any available functionality from all crypto libraries.

## Dependencies
- OpenSSL - instructions how to download and build OpenSSL can be found here *add link*

## Building RadJav Virtual Machine with KrispyCrypto with CMake
- The flag INCLUDE_CRYPTOGRAPHY must be enabled.
- The path OPENSSL_ROOT_DIR must be correctly set. It can be either set as an absolute or relative path. For example, a relative path might look like: *SOURCE_DIR/../../../../sdk/openssl/build/install*. This folder should contain (among others) *lib*, *bin* *include* directories. The *lib* or *bin* directory should contain OpenSSL shared libraries. For example, Linux build should have  *libcrypto.so*, *libssl.so* in the *lib* folder. Windows build will contain *libcrypto-<version>.dll* and *libssl-<version>dll* in the *bin* folder. 
- After running configuration, verify that OPENSSL_INCLUDE_DIR_PATH, OPENSSL_SSL_LIBRARY and OPENSSL_CRYPTO_LIBRARY paths are correct. If there are other installations of OpenSSL, it can happen that CMake will pick these if there was a mistake in specifying root path.
- KrispyCrypto_BUILD_TESTS can be left unchecked.
- KrispyCrypto_STATIC can be either checked or unchecked.


## General Syntax and Semantics
### Creating Objects
KrispyCryto objects exist in RadJav.Crypto namespace. In order to access a particular functionality, for example, encrypt some data, one would create a `Cipher` object.
```javascript
    var cipher = new RadJav.Crypto.Cipher({cryptoLibrary: "OpenSSL",
					   cipherAlgorithm: "aes-128-cbc",
					   secret: "Beer",
					   outputEncoding: "base64"});
```
Object constructor accepts a *key:value* map as an argument. As it can be seen from above example, the provided *key:value* map informs the KrispyCrypto interface about object specifics, here: the library (*OpenSSL*), which algorithm to use (*AES*), how to encode the output (*base64*), and what is the secret/password used in subsequent encryption calls.

Notes:
- Order of *key:value* pairs is not important,
- In this particular case, not all of these arguments are required (OpenSSL is right now a default library),
- Similarly, more arguments can be provided (to be explained later),
- If a constructor doesn't have all required information, an exception will be thrown,
- User can query the object for values of its parameters.

### Querying Library Capabilities
User can also query the class (e.g. `Hash`) for its capabilities. The `getCapabilities()` static method will return a hash which can be parsed for the information.

```javascript
    var capabilities = RadJav.Crypto.Hash.getCapabilities();
    
    var cryptoLib = capabilities.cryptoLibs.OpenSSL;
    for (var idx in cryptoLib.hashAlgorithms)
    {
        RadJav.Console.println(cryptoLib.hashAlgorithms[idx].name + ":\t" + cryptoLib.hashAlgorithms[idx].description);
    }
```
This code will yield an output like below.
```
blake2b512:     BLAKE2, 512bit
blake2s256:     BLAKE2, 256bit
md4:    MD4, 128bit
md5:    MD5, 128bit
mdc2:   MDC-2/Meyer-Schilling, 128bit
rmd160: RIPEMD-160, 160bit
...
```

### Making KrispyCrypto Objects Work for a Living
KrispyCrypto objects have both methods which return a promise, and synchronous/blocking methods. The later will typically have suffix `Sync` appended to the method name. The synchronous methods are convenient for simple/quick operations.
```javascript
var cipherResult = cipher.cipherSync("asdfasdf");
```

For dealing with streaming data, or large files, an approach which processes the data in smaller chunks, and utilizes both promises and synchronous calls, would be more convenient.
```javascript
    // var cipher = new RadJav.Crypto.CipherMultipart(...);
    
    cipher.update('1234567').then (
	function ()
	{
	    cipher.updateSync('890');
	    cipher.updateSync('abcdefgh');
	    var cipherResult = cipher.finalize();
        
        // Do something with cipherResult
	});
```

A question might present itself at this point: why does the interface differentiate between `Cipher` and `CipherMultipart` objects? This is because the multipart operation needs to store a status of encryption, whereas the simple `Cipher` is stateless in that regard. It might be error prone to mix both functionalities in the same object. *Note to RadJav team - we can of course decide to merge these in one object, but it should be done after some careful consideration.*



## Supported Functionality and Future
##### OpenSSL Functionality
- hash (including sha3)
- symmetric cipher
- symmetric decipher
- asymetric cryptography with RSA

##### To be added in not so distant future
- asymetric cryptography with ECDH (Diffie Hellman key exchange)
- asymetric cryptography with ECDSA (Digital Signature Algorithm)

##### A little bit more further down the road
- asymetric cryptography with ECIES (Integrated Encryption Scheme)

### Adding support for other crypto libraries
- support for hardware token access via PKCS11
- support for other crypto libraries such as NaCl/libsodium, Crypto++, etc.

# KrispyCrypto  Reference
## `Hash` Object
### Instantiation
```
var hash = new RadJav.Crypto.Hash({param:value, param1:value1, ...});
```
#### Required Parameters
- hashAlgorithm - name of the cryptographic hash algorithm to be applied. See below for the list of supported algorithms.


#### Other Supported Parameters
- cryptoLibrary [OpenSSL] - crypto library to use. Currently, only `OpenSSL` is supported.
- inputEncoding [binary] - encoding of input data, one of: `binary`, `hex`, `base64`.
- outputEncoding [binary] - encoding of output data, one of: `binary`, `hex`, `base64`.

#### Readable Properties
- *objectName*.cryptoLibrary - name of crypto library used by this object.
- *objectName*.hashAlgorithm - name of the hash algorithm used by this object.
- *objectName*.inputEncoding - name of expected format of input data.
- *objectName*.outputEncoding - name of the format of returned output.

#### Supported Algorithms for OpenSSL
- blake2b512: BLAKE2, 512bit
- blake2s256:     BLAKE2, 256bit
- md4:    MD4, 128bit
- md5:    MD5, 128bit
- mdc2:   MDC-2/Meyer-Schilling, 128bit
- rmd160: RIPEMD-160, 160bit
- sha1:   SHA-1, 160bit
- sha224: SHA-2, 224bit
- sha256: SHA-2, 256bit
- sha384: SHA-2, 384bit
- sha3_224:       SHA-3, 224bit
- sha3_256:       SHA-3, 256bit
- sha3_384:       SHA-3, 384bit
- sha3_512:       SHA-3, 312bit
- sha512: SHA-2, 512bit
- shake128:       SHA-3 Family, extendable-output function, 128bit strenth
- shake256:       SHA-3 Family, extendable-output function, 256bit strength
- sm3:    SM3 (Chinese Standard), similar to SHA-2, 256bit

### Methods
- hash(data) - returns a promise to perform the digest of `data`
- hashSync(data)  - returns the digest of `data`

### Examples
This example demonstrates how to get a digest of a string. It also shows how to query properties of the object (here `hashAlorithm`).
```javascript
try
{
    var digest = new RadJav.Crypto.Hash({cryptoLibrary: "OpenSSL", hashAlgorithm: "sha256", outputEncoding: "hex"});

    var digestResult = digest.digestSync('asdfadfadsf');
    RadJav.Console.println("Digest Algorithm: " + digest.hashAlgorithm);
    RadJav.Console.println("Digest Result: " + digestResult);
}
catch (error)
{
    RadJav.Console.println(error);
}
```

This example demonstrates how to get a digest of a string from the promise.
```javascript
try
{
    var digest = new RadJav.Crypto.Hash({cryptoLibrary: "OpenSSL", hashAlgorithm: "sha256", outputEncoding: "base64"});
    digest.digest('asdfadfadsf').then (function (data)
		   		     {
    					 alert("Done!\nHash Alg: " + digest.hashAlgorithm + "\nHash: " +
					       data);
				     });
}
catch (error)
{
    RadJav.Console.println(error);
}				     
```
