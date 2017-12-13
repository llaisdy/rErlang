-module(eri).

-export([start/0, start/1, connect/0, eval/1, stop/0]).
-export([init/1, parse/1, sum/2]).

start()->
    start(get_priv_dir() ++ "/bin/ERI-0.1").

start(ExtPrg) ->
    register(?MODULE, spawn_link(?MODULE, init, [ExtPrg])).

connect() -> 
    call_port({setup}).

-type r_sexp_type() :: atom().
-type r_result() :: list() | non_neg_integer().
-spec eval(string()) -> {ok, r_sexp_type(), r_result()} | {error, error, badarg}.
eval(X)->    
    case parse(X) of
	{ok,Result} -> call_port({eval,Result});
	_ -> {error}
    end.

stop() ->
    case call_port({stop}) of 
	{ok} ->
	    log("terminate R session~n"),
	    ?MODULE ! stop;
	{error} -> 
	    log("fail terminate R session~n"),
	    ?MODULE ! stop;
	_ ->
	    ?MODULE ! stop
    end.

init(ExtPrg)->
    process_flag(trap_exit, true),
    Port = open_port({spawn, ExtPrg}, [{packet,4}, binary]),
    loop(Port).

parse(X)->
    call_port({parse,X}).

sum(X,Y) ->
    call_port({sum, X, Y}).


%%%% private

get_priv_dir() ->
    case code:priv_dir(rerlang) of
	{error, bad_name} -> "./priv";
	Path -> Path
    end.

log(S) ->
    log(S, []).
log(S,L) ->
    io:format(S,L).

call_port(Msg) ->
    ?MODULE ! {call, self(), Msg},
    receive
	{?MODULE, Result}->
	    Result
    end.

loop(Port) ->
    receive
	{call, Caller, Msg} ->
	    Port ! {self(), {command, term_to_binary(Msg)}},
	    receive
		{Port, {data, Data}} ->
		    Caller ! {?MODULE, safe_b2t(Data)}
	    end,
	    loop(Port);
	stop ->
	    Port ! {self(), close},
	    receive
		{Port, closed} ->
		    exit(normal)
	    end;
	{'EXIT', Port, Reason} ->
	    exit({port_terminated, Reason})
    end.

safe_b2t(Bin) ->
    try binary_to_term(Bin) of
	Term ->
	     Term
    catch
	ExcClass:ExcPattern ->
	    {error, ExcClass, ExcPattern}
    end.
