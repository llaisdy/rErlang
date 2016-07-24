-module(eri).

-export([start/0, start/1, connect/0, eval/1, stop/0]).
-export([init/1, parse/1, sum/2]).

start()->
    start(get_priv_dir() ++ "/bin/ERI-0.1").

start(ExtPrg) ->
    register(?MODULE, spawn_link(?MODULE, init, [ExtPrg])).

connect() -> 
    call_port({setup}).

eval(X)->    
    case parse(X) of
	{ok,Result} -> call_port({eval,Result});
	_ -> {error}
    end.

stop() ->
    case call_port({stop}) of 
	{ok} ->
	    io:fwrite("terminate R session\n"),
	    ?MODULE ! stop;
	{error} -> 
	    io:fwrite("fail terminate R session\n"),
	    ?MODULE ! stop;
	_ ->
	    ?MODULE ! stop
    end.

init(ExtPrg)->
    process_flag(trap_exit, true),
    Port = open_port({spawn, ExtPrg}, [{packet,2}, binary]),
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

call_port(Msg) ->
    %% erlang:display("sending message: "),
    %% erlang:display(Msg),
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
		    %% erlang:display(Data),
		    Caller ! {?MODULE, binary_to_term(Data)}
	    end,
	    loop(Port);
	stop ->
	    erlang:display("Now actually trying to stop"),
	    Port ! {self(), close},
	    receive
		{Port, closed} ->
		    exit(normal)
	    end;
	{'EXIT', Port, Reason} ->
	    exit({port_terminated, Reason})
    end.
