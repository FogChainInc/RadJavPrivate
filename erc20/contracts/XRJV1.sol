// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "./IERC20.sol";
import "./extensions/IERC20Metadata.sol";
import "./utils/Context.sol";

/**
 * The XRJ
 */
contract XRJ is Context, IERC20, IERC20Metadata
{
	/**
	 * The name of the XRJ token.
	 */
	string private _name;
	/**
	 * The XRJ symbol.
	 */
	string private _symbol;
	/**
	 * The total supply of tokens.
	 */
	uint256 private _totalSupply;
	/**
	 * The number of decimals.
	 */
	uint8 public constant _decimals = 18;
	/**
	 * The token's account balances.
	 */
	mapping(address => uint) private _balances;
	/**
	 * The token's account allowances.
	 */
	mapping(address => uint) private _allowances;

	constructor () public {
		_name = "RadJav XRJV1 Token";
		_symbol = "XRJV1";
		_totalSupply = 110411200 * 10**uint(_decimals);
		_balances[tx.origin] = 10000;
	}

	/**
	 * @dev Returns the name of the token.
	 */
	function name () public view virtual override returns (string memory)
	{
		return (_name);
	}

	/**
	 * @dev Returns the symbol of the token.
	 */
	function symbol () public view virtual override returns (string memory)
	{
		return (_symbol);
	}

	/**
	 * @dev Returns the decimals of the token.
	 */
	function decimals () public view virtual override returns (uint8)
	{
		return (18);
	}

	/**
	 * @dev Returns the decimals of the token.
	 */
	function totalSupply () public view virtual override returns (uint256)
	{
		return (_totalSupply);
	}

	/**
	 * @dev Returns the decimals of the token.
	 */
	function balanceOf (address account) public view virtual override returns (uint256)
	{
		return (_balances[account]);
	}

	/**
	 * @dev Transfer tokens.
	 */
	function transfer (address recipient, uint256 amount) public virtual override returns (bool)
	{
		_transfer (_msgSender (), recipient, amount);

		return (true);
	}

	/**
	 * @dev Get the allowance for an owner.
	 */
	function allowance (address owner, address spender) public view virtual override returns (uint256)
	{
		return _allowances[owner][spender];
	}

	/**
	 * @dev Get the allowance for an owner.
	 */
	function approve (address spender, uint256 amount) public virtual override returns (bool)
	{
		_approve (_msgSender (), spender, amount);

		return (true);
	}

	/**
	 * @dev Transfer.
	 */
	function transferFrom (address sender, address recipient, uint256 amount) public virtual override returns (bool)
	{
		_transfer (sender, recipient, amount);

		uint256 currentAllowance = _allowances[sender][_msgSender ()];
		require (currentAllowance >= amount, "ERC20: transfer amount exceeds allowance");
		_approve (sender, _msgSender (), currentAllowance - amount);

		return (true);
	}

	/**
	 * @dev Transfer.
	 */
	function increaseAllowance (address spender, uint256 addedValue) public virtual returns (bool)
	{
		_approve (_msgSender (), spender, _allowances[_msgSender ()][spender] + addedValue);

		return (true);
	}

	/**
	 * @dev Transfer.
	 */
	function decreaseAllowance (address spender, uint256 subtractedValue) public virtual returns (bool)
	{
		uint256 currentAllowance = _allowances[_msgSender ()][spender];
		require (currentAllowance >= subtractedValue, "ERC20: decreased allowance below zero");
		_approve (_msgSender (), spender, currentAllowance - subtractedValue);

		return (true);
	}

	/**
	 * @dev Transfer.
	 */
	function _transfer (address sender, address recipient, uint256 amount) internal virtual
	{
		require (sender != address (0), "ERC20: transfer from the zero address");
		require (recipient != address (0), "ERC20: transfer to the zero address");

		_beforeTokenTransfer (sender, recipient, amount);

		uint256 senderBalance = _balances[sender];
		require (senderBalance >= amount, "ERC20: transfer amount exceeds balance");
		_balances[sender] = senderBalance - amount;
		_balances[recipient] += amount;

		emit Transfer (sender, recipient, amount);
	}

	/**
	 * @dev Transfer.
	 */
	function _approve (address owner, address spender, uint256 amount) internal virtual
	{
		require (owner != address (0), "ERC20: approve from the zero address");
		require (spender != address (0), "ERC20: approve to the zero address");

		_allowances[owner][spender] = amount;
		emit Approval (owner, spender, amount);
	}

	/**
	 * @dev Transfer.
	 */
	function _beforeTokenTransfer(address from, address to, uint256 amount) internal virtual
	{
	}
}
