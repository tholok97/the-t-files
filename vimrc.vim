" Laget av: Thomas Løkkeborg

"------------------------------------- PRE ----------------------------------{{{

" netwr greier. skrur av banneret, skrur på tre, setter bredden på ø-vinduet og
" ignorerer swp-filer 
let g:netrw_banner = 0
let g:netrw_liststyle = 3
let g:netrw_list_hide= '.*\.swp$'

" skrur av highlighting av forrige søk
nohlsearch

" skrur på filtype
filetype on

" sett opp (gui) font-greier om det ikke allerede er satt opp
if !exists("g:font_size")
	let g:font_size = 10
	silent execute "normal! :set guifont=Consolas:h" . font_size . "\<cr>"
endif

"}}}
"-------------------------------- ABBREVIATIONS -----------------------------{{{
iabbrev @@ thomahl@stud.ntnu.no
iabbrev intmain #include <iostream><cr><cr>using namespace std;<cr><cr>int
		\ main() {<cr><cr><cr><cr>return 0;<cr>}

"}}}
"--------------------------------- FUNKSJONER -------------------------------{{{

" funksjoner som erstatter ord under cursor med ord gitt ved input
function! SearchAndReplaceCword()
	let l:word = "\\<" . expand("<cword>") . "\\>"
	let l:replacement = input("Erstatning? ")
	execute ":%s/" . l:word . "/" . l:replacement . "/g"
	normal 
endfunction

" (denne kjører med gc, fra current line)
function! SearchAndReplaceCwordC()
	let l:word = "\\<" . expand("<cword>") . "\\>"
	let l:replacement = input("Erstatning? ")
	execute "%s/" . l:word . "/" . l:replacement . "/gc"
	normal 
endfunction

" søk etter noe gitt ved en motion med vimgrep. Søker igjennom .cpp, .py og .pdb
" -filer i samme mappe
function! s:GrepOperator(type)
	let g:quickfix_return_to_window = winnr()
	let g:quickfix_is_open = 1
	let l:saved_unnamed_register = @@

	if a:type ==# 'v'
		normal! `<v`>y
	elseif a:type ==# 'char'
		normal! `[v`]y
	else
		return
	endif

	echom "Søker etter: " . @@
	silent execute "vimgrep /" . @@ . "/j *.cpp *.h *.py *.pdb *.md"
	copen

	let @@ = l:saved_unnamed_register
endfunction

" lager tittel basert på kommenteringsstil (param) og input fra bruker
function! MakeTitle(cmt)
	let l:title = input("Tittel? ")
	let l:total = 80 - len(title) - len(a:cmt) - 2
	let l:extra = 0
	if (len(l:title) + len(a:cmt)) % 2 ==# 1
		let l:extra = 1
	endif
	execute "normal! o\<esc>o\<esc>ko" . a:cmt . repeat('-', total/2) . " " .
			\toupper(l:title) . " " . repeat('-', total/2 + l:extra) . "\<esc>"
endfunction

" funksjon som åpner / lukker quickfix-vinduet
let g:quickfix_is_open = 0
function! QuickfixToggle()
	if g:quickfix_is_open
		cclose
		let g:quickfix_is_open = 0
		execute g:quickfix_return_to_window . "wincmd w"
	else
		let g:quickfix_return_to_window = winnr()
		copen
		let g:quickfix_is_open = 1
	endif
endfunction

" returnerer x 923'er. x er antall som kan passe fint på skjermen
function! NiTjueTre()
	return ' ' . repeat('923 ', winwidth(0)/8)
endfunction

" øk (gui) font-størrelsen
function! IncGUIFontSize()
	let g:font_size += 1	
	execute "normal! :set guifont=Consolas:h" . g:font_size . "\<cr>"
endfunction

" minske (gui) font-størrelsen
function! DecGUIFontSize()
	let g:font_size -= 1	
	execute "normal! :set guifont=Consolas:h" . g:font_size . "\<cr>"
endfunction

" toggle markdown-modus
let g:is_in_md_mode = 0
function! ToggleMarkdownMode()
	if (g:is_in_md_mode == 0)
		set wrap
		set linebreak
		set nonumber
		set laststatus=0
		let g:is_in_md_mode = 1
	else
		set nowrap
		set nolinebreak
		set number
		set laststatus=2
		let g:is_in_md_mode = 0
	endif
endfunction

"}}}
"------------------------------- SETTER VERDIER -----------------------------{{{

" PROSJEKT PROSJEKT PROSJEKT PROSJEKT skal være 4
set tabstop=2
set softtabstop=2
set shiftwidth=2
set expandtab
" PROSJEKT PROSJEKT PROSJEKT PROSJEKT skal være 4

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
	autocmd Filetype cpp setlocal cindent
augroup END

"}}}
"---------------------- KOSMETISK (syntax, highlighting) --------------------{{{

" syntax, colorscheme og farge på grenselinje
syntax on
colorscheme darkblue
highlight ColorColumn ctermbg=6 guibg=darkRed

" utgangsfarge på statuslinja
highlight ntt ctermbg=0 guibg=Black ctermfg=2 guifg=darkgreen
highlight StatusLine ctermfg=0 guifg=Black ctermbg=2 guibg=darkgreen

" MIDLERTIDIG FIX. gjør colorschemet mer leslig
if has("gui_running")
	highlight Normal guibg=Black
	highlight Visual guifg=Red guibg=Black
  highlight Comment guifg=darkred
  highlight ColorColumn guibg=grey
endif

" setter at statuslinja alltid skal vises
set laststatus=2

" farge på statuslinja skifter ved modusskifte, og vimrc folding
if version >= 700
	augroup statuslineGroup
		autocmd!
		autocmd InsertEnter * highlight StatusLine ctermfg=0 guifg=Black
				\ ctermbg=5 guibg=purple
		autocmd InsertEnter * highlight ntt ctermbg=0 guibg=Black ctermfg=5
				\ guifg=purple
		autocmd InsertLeave * highlight StatusLine ctermfg=0 guifg=Black
				\ ctermbg=2 guibg=darkgreen
		autocmd InsertLeave * highlight ntt ctermbg=0 guibg=Black ctermfg=2
				\ guifg=darkgreen
		autocmd CursorHold * echo "Whatareyouwaitingfoooooooooooooooooooooooor!"
    autocmd FileType vim setlocal foldmethod=marker
		autocmd Filetype vim setlocal foldlevelstart=0
		autocmd FileType vim setlocal foldcolumn=2
	augroup END
endif


" bygger statuslinja (venstre side)
set statusline=%#Statusline#\  " så uvalgte vinduer ikke blir hvite
set statusline+=%m				" vis om filen er endret
set statusline+=%f         		" stien til filen
set statusline+=\ -\ 			" separator
set statusline+=FileType:		" tekst
set statusline+=%y				" FileTypen til filen
set statusline+=\ \ \ %#ntt#	" spacing og start ny highlight-gruppe 	(923)
set statusline+=%{NiTjueTre()}	" skriv ut 923 via funksjon
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
nnoremap <Up>		  <C-W>+
nnoremap <Down>		<C-W>-
nnoremap <Left>		<C-W><
nnoremap <Right>	<C-W>>

" Function key -kommandoer (F1: cmd, F2: vimrc, F3: source vimrc, F4: md-mode)
nnoremap <F1>		:silent !start cmd<cr>
nnoremap <F2>		:sp $myvimrc<cr>
nnoremap <F3>		:so $myvimrc<cr>
nnoremap <F4>		:call ToggleMarkdownMode()<cr>

" div. modifikasjoner til bevegelser i vim
nnoremap j gj
nnoremap k gk
inoremap jk <esc>
nnoremap Y y$
nnoremap H _
nnoremap L $
nnoremap <c-e> <c-e>j
nnoremap <c-y> <c-y>k

" skifte tab
nnoremap <leader><cr> gt
nnoremap <leader><tab> gT
nnoremap <leader><bs> :$tabnew 

" øk/minske (gui) font-størrelsen
nnoremap <leader><up>		  :call IncGUIFontSize()<cr>
nnoremap <leader><down>		:call DecGUIFontSize()<cr>

" vindu-greier:
nnoremap <leader><leader> <c-w><c-w>
nnoremap <leader>= <c-w>=
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
nnoremap <leader>f :sp 
nnoremap <leader>F :sp<cr>
nnoremap <leader>d :rightb vsp 
nnoremap <leader>D :rightb vsp<cr>
nnoremap <leader>t <c-w>r

" søke-greier:
nnoremap <leader>r :call SearchAndReplaceCword() <cr>
nnoremap <leader>e :call SearchAndReplaceCwordC() <cr>
nnoremap <leader>y :nohlsearch<cr>

" mark. 2 stykk. Trykk én gang for å sette, og to for å gå til
nnoremap <leader>s ma
nnoremap <leader>ss `a
nnoremap <leader>a mb
nnoremap <leader>aa `b

" macro (start med qq, slutt med q, kjør med <leader><cr>)
nnoremap <leader><cr> @q

" vimgrep greier:
nnoremap <leader>g :set operatorfunc=<SID>GrepOperator<cr>g@
vnoremap <leader>g :<c-u>call <SID>GrepOperator(visualmode())<cr>
nnoremap <leader>m :call QuickfixToggle()<cr>
nnoremap <leader>n :cnext<cr>
nnoremap <leader>p :cprevious<cr>

" ny linje under/over uten å gå inn i insert-mode
nnoremap <leader>o o<esc>
nnoremap <leader><s-o> O<esc>

" vis/gjem trailing whitespace
nnoremap <leader>u :match Error /\v\s+$/<cr>
nnoremap <leader>i :match none<cr>

" flytt linje én opp, én ned
nnoremap <leader>+ ddp
nnoremap <leader>- ddkP

" omring ord i hermetegn
nnoremap <leader>2 mqviw<esc>a"<esc>bi"<esc>`ql
nnoremap <leader>" mqF"xf"x`qh
nnoremap <leader>' mqviw<esc>a'<esc>bi'<esc>`ql
nnoremap <leader>* mqF'xf'x`qh

" kommenteringssnarveier:
"...{{{
augroup commenting_shortcuts
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
	autocmd FileType tex nnoremap <buffer> <leader>b :call
			\ MakeTitle("%")<cr>
	autocmd FileType tex nnoremap <buffer> <leader>c 	mq0i%<esc>`ql
	autocmd FileType tex nnoremap <buffer> <leader>x 	mq0x`qh
	autocmd FileType tex vnoremap <buffer> <leader>c 	omq0<S-i>%<esc>`ql
	autocmd FileType tex vnoremap <buffer> <leader>x 	omq0o0x`qh
augroup END
"}}}

"}}}