const ConvertLib = artifacts.require("ConvertLib");
const XRJV1 = artifacts.require("XRJV1");

module.exports = function(deployer) {
  deployer.deploy(ConvertLib);
  deployer.link(ConvertLib, XRJV1);
  deployer.deploy(XRJV1);
};
