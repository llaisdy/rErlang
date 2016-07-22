-module(erc).

-export([init/0,
	 source/1]).

init() ->
    eri:start(),
    eri:connect().

source(Fn) ->
    eri:eval("source(\"" ++ Fn ++"\")").
