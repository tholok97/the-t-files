"------------------------------------- PRE ----------------------------------{{{

" skrur på filtype
filetype on

"}}}
"-------------------------------- ABBREVIATIONS -----------------------------{{{

iabbrev intmain #include <iostream><cr><cr>using namespace std;<cr><cr>int
		\main() {<cr><cr><cr><cr>return 0;<cr>}

"}}}
"--------------------------------- FUNKSJONER -------------------------------{{{

" funksjoner som erstatter ord under cursor med ord gitt ved input
function! SearchAndReplaceCword()
	let word = "\\<" . expand("<cword>") . "\\>"
	let replacement = input("Erstatning? ")
	execute ":%s/" . word . "/" . replacement . "/g"
	normal 
endfunction

" (denne kjører med gc, fra current line)
function! SearchAndReplaceCwordC()
	let word = "\\<" . expand("<cword>") . "\\>"
	let replacement = input("Erstatning? ")
	execute "%s/" . word . "/" . replacement . "/gc"
	normal 
endfunction

" søk etter noe gitt ved en motion med vimgrep. Søker igjennom .cpp, .py og .pdb
" -filer i samme mappe
function! s:GrepOperator(type)
	let savedUnnamedRegister = @@

	 if a:type ==# 'v'
			normal! `<v`>y
		elseif a:type ==# 'char'
			normal! `[y`]y
		else
			return
		endif

	echom "Søker etter: " . @@
	silent execute "vimgrep /" . @@ . "/j *.cpp *.py *.pdb"
	copen

	let @@ = savedUnnamedRegister
endfunction

" lager tittel basert på kommenteringsstil (param) og input fra bruker
function! MakeTitle(cmt)
	let title = input("Tittel? ")
	let total = 80 - len(title) - len(a:cmt) - 2
	let extra = 0
	if (len(title) + len(a:cmt)) % 2 ==# 1
		let extra = 1
	endif
	execute "normal! o" . a:cmt . repeat('-', total/2) . " " . toupper(title) . 
			\" " . repeat('-', total/2 + extra) . "\<esc>"
endfunction

"}}}
"------------------------------- SETTER VERDIER -----------------------------{{{

set tabstop=4
set softtabstop=4
set shiftwidth=4
set number
set backspace=2
set cc=80
set scrolloff=10
set encoding=utf-8
set smartcase
set ignorecase
set autochdir
"set nostartofline

set wildmenu
set hlsearch
set incsearch

augroup cppSpecific
	autocmd Filetype cpp set cindent
augroup END

"}}}
"---------------------- KOSMETISK (syntax, highlighting) --------------------{{{

" syntax, colorscheme og farge på grenselinje
syntax on
colorscheme darkblue
highlight ColorColumn ctermbg=6

" farge på nitjuetre-felt
highlight ntt ctermbg=black ctermfg=darkgrey

" setter at statuslinja alltid skal vises
set laststatus=2

" farge på statuslinja skifter ved modusskifte, og vimrc folding
if version >= 700
	augroup statuslineGroup
		autocmd VimEnter * highlight StatusLine term=reverse ctermfg=0 
				\ctermbg=2 gui=bold,reverse
		autocmd InsertEnter * highlight StatusLine term=reverse ctermbg=5 
				\gui=undercurl guisp=Magenta
		autocmd InsertLeave * highlight StatusLine term=reverse ctermfg=0 
				\ctermbg=2 gui=bold,reverse
		autocmd CursorHold * echo "Whatareyouwaitingfoooooooooooooooooooooooor!"
    	autocmd FileType vim setlocal foldmethod=marker
		autocmd Filetype vim setlocal foldlevelstart=0
		autocmd FileType vim setlocal foldcolumn=2
	augroup END
endif

" bygger statuslinja (venstre side)
set statusline=%#Statusline# 	" så uvalgte vinduer ikke blir hvite
set statusline+=%m				" vis om filen er endret
set statusline+=%f         		" stien til filen
set statusline+=\ -\ 			" separator
set statusline+=FileType:		" tekst
set statusline+=%y				" FileTypen til filen
set statusline+=\ \ \ %#ntt#	" spacing og start ny highlight-gruppe 	(923)
set statusline+=923\ 923\ 923\ 923\ 923\ 923\ 923\ 923\ 923\ 923\ 923 "	(923)
set statusline+=%#Statusline#	" endre tilbake til default

" (høyre side)
set statusline+=%=		  		" hopp til andre siden
set statusline+=%4l		  		" linja vi er på
set statusline+=/				" /
set statusline+=%-4L	   		" totalt antall linjer

"}}}
"----------------------------------- MAPPING --------------------------------{{{

" space som leder
let mapleader="\<space>"

" endre størrelse på vindu med piltaster
nnoremap <Up>		<C-W>+
nnoremap <Down>		<C-W>-
nnoremap <Left>		<C-W><
nnoremap <Right>	<C-W>>

" åpne / laste vimrc
nnoremap <F2>		:sp $myvimrc <cr>
nnoremap <F3>		:so $myvimrc <cr>

" div. modifikasjoner til bevegelser i vim
nnoremap j gj
nnoremap k gk
inoremap jk <esc>
nnoremap Y y$
nnoremap + ddp
nnoremap - ddkP
nnoremap H _
nnoremap L $
nnoremap <c-e> <c-e>j
nnoremap <c-y> <c-y>k

" vindu-greier:
nnoremap <leader><leader> <c-w><c-w>
nnoremap <leader>h <c-w>h
nnoremap <leader>j <c-w>j
nnoremap <leader>k <c-w>k
nnoremap <leader>l <c-w>l
nnoremap <leader>H <c-w>H
nnoremap <leader>J <c-w>J
nnoremap <leader>K <c-w>K
nnoremap <leader>L <c-w>L
nnoremap <leader>w <c-w>o
nnoremap <leader>q <c-w>q
nnoremap <leader>f :sp . <cr>
nnoremap <leader>F :sp<cr>
nnoremap <leader>d :rightb vsp . <cr>
nnoremap <leader>D :rightb vsp<cr>
nnoremap <leader>t <c-w>r

" søke-greier:
nnoremap <leader>r :call SearchAndReplaceCword() <cr>
nnoremap <leader>e :call SearchAndReplaceCwordC() <cr>
nnoremap <leader>y :nohlsearch<cr>

" mark
nnoremap <leader>s ma
nnoremap <leader>a `a

" macro
nnoremap <leader><bs> qq
nnoremap <leader><cr> @q

" vimgrep greier:
nnoremap <leader>g :set operatorfunc=<SID>GrepOperator<cr>g@
vnoremap <leader>g :<c-u>call <SID>GrepOperator(visualmode())<cr>
nnoremap <leader>m :cclose<cr>
nnoremap <leader>n :cnext<cr>
nnoremap <leader>p :cprevious<cr>

" ny linje under/over uten å gå inn i insert-mode
nnoremap <leader>o o<esc>
nnoremap <leader><s-o> O<esc>

" vis/gjem trailing whitespace
nnoremap <leader>u :match Error /\v\s+$/<cr>
nnoremap <leader>i :match none<cr>

" motion for å velge hele filen
onoremap a :<c-u>normal! ggVG<cr>

" omring ord i hermetegn
nnoremap <leader>2 mqviw<esc>a"<esc>bi"<esc>`ql
nnoremap <leader>" mqF"xf"x`qh
nnoremap <leader>' mqviw<esc>a'<esc>bi'<esc>`ql
nnoremap <leader>* mqF'xf'x`qh

" kommenteringssnarveier:
"...{{{
augroup commentingShortcuts
	autocmd!
	autocmd FileType cpp nnoremap <buffer> <leader>b :call
			\ MakeTitle("//")<cr>
	autocmd FileType cpp nnoremap <buffer> <leader>c 		mq0i//<esc>`qll
	autocmd FileType cpp nnoremap <buffer> <leader>x 		mq0xx`qhh
	autocmd FileType cpp vnoremap <buffer> <leader>c 		omq0<S-i>//<esc>`qll
	autocmd FileType cpp vnoremap <buffer> <leader>x 		omq0o0lx`qhh
	autocmd FileType python nnoremap <buffer> <leader>b :call
			\ MakeTitle("#")<cr>
	autocmd FileType python nnoremap <buffer> <leader>c 	mq0i#<esc>`ql
	autocmd FileType python nnoremap <buffer> <leader>x 	mq0x`qh
	autocmd FileType python vnoremap <buffer> <leader>c 	omq0<S-i>#<esc>`ql
	autocmd FileType python vnoremap <buffer> <leader>x 	omq0o0x`qh
	autocmd FileType vim nnoremap <buffer> <leader>b :call
			\ MakeTitle("\"")<cr>
	autocmd FileType vim nnoremap <buffer> <leader>c 		mq0i"<esc>`ql
	autocmd FileType vim nnoremap <buffer> <leader>x 		mq0x`qh
	autocmd FileType vim vnoremap <buffer> <leader>c 		omq0<S-i>"<esc>`ql
	autocmd FileType vim vnoremap <buffer> <leader>x 		omq0o0x`qh
	autocmd FileType prolog nnoremap <buffer> <leader>b :call
			\ MakeTitle("%")<cr>
	autocmd FileType prolog nnoremap <buffer> <leader>c 	mq0i%<esc>`ql
	autocmd FileType prolog nnoremap <buffer> <leader>x 	mq0x`qh
	autocmd FileType prolog vnoremap <buffer> <leader>c 	omq0<S-i>%<esc>`ql
	autocmd FileType prolog vnoremap <buffer> <leader>x 	omq0o0x`qh
augroup END
"}}}

"}}}
