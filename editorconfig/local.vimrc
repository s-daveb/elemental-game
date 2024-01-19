 
" editorconfig/local.vimrc
" Copyright © 2023-2024 Saul D. Beniquez  @{
"
" Redistribution and use in source and binary forms, with or without
" modification, are permitted provided that the following conditions are met:
"
" 1. Redistributions of source code must retain the above copyright notice,
"    this list of conditions and the following disclaimer.
"
" 2. Redistributions in binary form must reproduce the above copyright notice,
"    this list of conditions and the following disclaimer in the documentation
"    and/or other materials provided with the distribution.
"
" THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
" IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
" ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
" LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
" CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
" SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
" INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
" CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
" ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
" POSSIBILITY OF SUCH DAMAGE. @}

set foldmethod=expr
  \ foldexpr=lsp#ui#vim#folding#foldexpr()
  \ foldtext=lsp#ui#vim#folding#foldtext()


let s:build_dir = 'vim-debug'
let s:build_cores = 2
let s:make_args =  '-C '. s:build_dir . ' -j ' . s:build_cores . ' ctest'

let s:cmake_path = system('which ' . 'cmake')
let s:ninja_path  = system('which ' . 'ninja')

let s:cmake_generator = 'Unix Makefiles'
if (s:ninja_path != '')
	let s:cmake_generator = 'Ninja'
endif

if (s:cmake_path != '')
	let s:cmake_call = s:cmake_path .
				\' -B ' . s:build_dir .
				\' -D ' . 'CMAKE_BUILD_TYPE=Debug' .
				\' -G ' . s:cmake_generator
endif

let s:make_call = 'make ' . s:make_args
let s:ninja_call = 'ninja ' . s:make_args

if ! get(s:, 'defined', 0) " -- prevents the function from being redefined after compiling
function! BuildDebug()
	let s:defined = 1

	wall

	if (s:ninja_path != '')
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/build.ninja'))
		\ )


			exec ':Dispatch ' . s:cmake_call
		endif

		set makeprg='ninja'
		exec ':Make ' . s:make_args
	else
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/Makefile'))
		\ )
			exec ':Dispatch ' . s:cmake_call
		endif

		if exists(':Make')
			exec ':Make ' . s:make_args
			cd ..
		else
			exec 'make ' . s:make_args
			vert botright copen
			vert resize +100
		endif
	endif

endfunction

function RunTests()
	let s:defined = 1
	if (s:ninja_path != '')
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/build.ninja'))
		\ )


			exec ':Dispatch ' . s:cmake_call
		endif

		set makeprg='ninja'
		exec ':Make ' . s:make_args. ' ctest'
	else
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/Makefile'))
		\ )
			exec ':Dispatch ' . s:cmake_call
		endif

		if exists(':Make')
			exec ':Make ' . s:make_args . ' ctest'
			cd ..
		else
			exec 'make ' . s:make_args . ' ctest'
			vert botright copen
			vert resize +100
		endif
	endif

endfunction
endif

nnoremap <leader>bd :call BuildDebug()<CR>
nnoremap <leader>rt :call RunTests()<CR>

set path+=src
set path+=include
set path+=app


" vim: set ts=4 sts=4 noet sw=4 foldmethod=marker foldmarker=@{,@} :
