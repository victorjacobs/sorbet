#!/bin/bash

tmp="$(mktemp -d)"
cwd="$(pwd)"
cd "$tmp" || exit 1
separator() {
    echo -------------------------
}

cp "$cwd"/test/cli/suggest-typed-true/*.rb "$tmp/"


"$cwd/main/sorbet" --silence-dev-message --suggest-typed --typed=false --error-white-list=7022 does-not-exist.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message --suggest-typed --typed=true --error-white-list=7022 does-not-exist.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict does-not-exist.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-true.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-ignore.rb 2>&1
cat suggest-typed-ignore.rb
"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-ignore.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-false.rb 2>&1
cat suggest-typed-false.rb
"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-false.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-true.rb 2>&1
cat suggest-typed-true.rb
"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-true.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-strict.rb 2>&1
cat suggest-typed-strict.rb
"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-strict.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-strong.rb 2>&1
cat suggest-typed-strong.rb
"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-strong.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict empty.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-with-too-low.rb 2>&1
cat suggest-typed-with-too-low.rb
separator

"$cwd/main/sorbet" --silence-dev-message --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-with-too-low.rb 2>&1
separator

"$cwd/main/sorbet" --silence-dev-message -a --suggest-typed --error-white-list=7022 --typed=strict suggest-typed-already-ignore.rb 2>&1
cat suggest-typed-already-ignore.rb
separator

rm -r "$tmp"
