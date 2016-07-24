%%-*- mode: erlang -*-

{application, rerlang, [
	{description, "rErlang"},
	{vsn, "0.0.1"},
	{id, "git"},
	{modules, [eri]},
	{registered, []},
	{applications, 
	 [compiler,
	  kernel,
	  stdlib,
	  syntax_tools
	]},
	{env, [
	      ]}
]}.
