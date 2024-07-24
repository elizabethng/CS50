import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
@login_required
def index():
    """Show portfolio of stocks"""
    # db = SQL("sqlite:///week9/finance/finance.db")
    holdings = db.execute("""
                    SELECT symbol, sum(shares) as n_shares
                    FROM "transactions"
                    WHERE userid = ?
                    GROUP BY symbol
                    HAVING n_shares > 0
                    """, session["user_id"])

    # Get the current cash holdings
    cash = cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    total = cash

    # Get the current stock price
    for stock in holdings:
        stock['price'] = lookup(stock['symbol'])['price']
        stock['value'] = stock['n_shares']*stock['price']
        total += stock['value']

    return render_template("index.html", holdings=holdings, cash=cash, total=total)


@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    """Change password"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure new password was submitted
        elif not request.form.get("newpassword"):
            return apology("must provide new password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm new password", 400)

        # Query database for current password
        rows = db.execute(
            "SELECT * FROM users WHERE id = ?", session["user_id"]
        )

        # Check current password
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid password", 400)

        # Check that new password matches confirmation
        if request.form.get("newpassword") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Update new password in database
        hash = generate_password_hash(request.form.get("newpassword"))
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, session["user_id"])

        # Redirect user to home page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("account.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure submitted symbol is valid
        if not request.form.get("symbol"):
            return apology("must provide valid symbol", 400)

        # Get current price of stock
        result = lookup(request.form.get("symbol"))
        if not result:
            return apology("must provide valid symbol", 400)

        # Ensure number of shares is valid
        if not request.form.get("shares"):
            return apology("must provide shares > 0", 400)

        try:
            # not sure if int or float is correct for API
            shares = float(request.form.get("shares"))
            if shares <= 0:
                return apology("must provide shares > 0", 400)
            if shares % 1 != 0:
                return apology("cannot provide fractional shares", 400)
        except:
            return apology("must provide shares > 0", 400)

        # Query database for available cash for user
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']

        # Check if user has enough cash to buy
        newcash = cash - result['price']*shares
        if newcash < 0:
            return apology("insufficient funds", 402)

        # Record transaction and deduct funds
        db.execute("INSERT INTO transactions (userid, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], "buy", result['symbol'], shares, result['price'])

        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])

        # Redirect user to home page upon completion
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history", methods=["GET"])
@login_required
def history():
    """Show history of transactions"""
    # db = SQL("sqlite:///week9/finance/finance.db")
    transactions = db.execute("""
                    SELECT *
                    FROM "transactions"
                    WHERE userid = ?
                    """, session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide valid symbol", 400)

        return redirect(url_for("quoted", symbol=symbol))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


# @app.route("/quoted", methods=["POST"])
@app.route("/quote/<symbol>", methods=["GET", "POST"])
@login_required
def quoted(symbol):
    """Display a valid quote or error message"""
    # Look up the desired stock
    result = lookup(symbol)

    # If an incorrect name is entered, lookup returns nothing
    if result is None:
        return apology("provide a valid symbol", 400)
    else:
        return render_template("quoted.html", quote=result)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register a new user."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Query database for username, give an error if the username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Check if username already exists then save password
        if len(rows) > 0:
            return apology("username already exists", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure that password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Ensure that password and confirmation match
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Hash the password and submit to DB
        hash = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   request.form.get("username"), hash)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get the list of stocks currently owned
    holdings = db.execute("""
                    SELECT symbol, sum(shares) as n_shares
                    FROM "transactions"
                    WHERE userid = ?
                    GROUP BY symbol
                    HAVING n_shares > 0
                    """, session["user_id"])

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        # Check that the user selected a stock (ensure submitted symbol is valid)
        if not symbol:
            return apology(f"must provide valid symbol: {symbol}", 400)
            # return apology("must provide valid symbol", 403)

        # Check that the user owns the stock
        # [ ] is this not already done by using the dropdown menu?

        # Ensure number of shares is valid
        if not shares:
            return apology("must provide shares > 0", 400)

        try:
            shares = float(shares)  # not sure if int or float is correct for API
            if shares <= 0:
                return apology("must provide shares > 0", 400)
        except:
            return apology("must provide shares > 0", 400)

        # Ensure that user owns that many shares
        for stock in holdings:
            if stock['symbol'] == symbol:
                if shares > stock['n_shares']:
                    return apology("insufficient shares", 400)

        # Get current price of stock
        result = lookup(symbol)

        # Query database for current cash for user
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']

        # Check if user has enough cash to buy
        newcash = cash + result['price']*shares

        # Record transaction and increment funds
        db.execute("INSERT INTO transactions (userid, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], "sell", result['symbol'], -shares, result['price'])

        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])

        # Redirect user to home page upon completion
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", holdings=holdings)
