-module(erc).

-export([init/0]).

init() ->
    eri:start(),
    eri:connect().
