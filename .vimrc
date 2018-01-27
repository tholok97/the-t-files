" Laget av: Thomas Løkkeborg


"------------------------------------- PRE ----------------------------------{{{

" valg for oppførsel av vimrc
let g:spaces_in_a_tab = 4

" netwr greier. skrur av banneret, skrur på tre, setter bredden på ø-vinduet og
" ignorerer swp-filer 
let g:netrw_banner = 0
let g:netrw_list_hide= '.*\.swp$'

" skrur av highlighting av forrige søk
nohlsearch

" skrur på filtype og (lar plugin leke med indent (??))
filetype on
filetype plugin indent on

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

" returnerer x 923'er. x er antall som kan passe fint i statuslinja
function! NiTjueTre()
    return ' ' . repeat('923 ', winwidth(0)/8)
endfunction

" øk (gui) font-størrelsen. (minsk ved negativ parameter)
function! IncGUIFontSize(how_much_to_change_size)
    let g:font_size += a:how_much_to_change_size
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
" toggle quickfix vinduet (fra internett)
function! QuickfixToggle() 
    for i in range(1, winnr('$')) 
        let bnum = winbufnr(i) 
        if getbufvar(bnum, '&buftype') == 'quickfix' 
            cclose 
            return 
        endif 
    endfor 

    copen 
endfunction 

" åpne matchende cpp/h fil horisontalt eller vertikalt
function! OpenMatchingFile(is_vertical)
    if a:is_vertical
      let l:split_type = "vsp"  
    else
      let l:split_type = "sp"  
    endif
    if expand("%:e") == "h"
        execute "normal! :rightb " . l:split_type . " " . expand("%:r") . 
                \".cpp\<cr>" 
    elseif expand("%:e") == "cpp"
        execute "normal! :rightb " . l:split_type . " " . expand("%:r") . 
                \".h\<cr>" 
    else
        echoerr "OpenMatchingFile error: Ikke .h/.cpp fil!"
    endif
endfunction

"}}}
"------------------------------- SETTER VERDIER -----------------------------{{{

" set tabgreier basert på PRE-variabel
let &tabstop=g:spaces_in_a_tab
let &softtabstop=g:spaces_in_a_tab
let &shiftwidth=g:spaces_in_a_tab

" tabs blir spaces
set expandtab

" div
set number
set backspace=2
set cc=80
set scrolloff=3 " was 10
set encoding=utf-8
set smartcase
set ignorecase
set autochdir
set wildmenu
"set hlsearch
set incsearch
set cursorline
set showcmd

" setter at statuslinja alltid skal vises
set laststatus=2

set autowrite

" language specific settings
augroup lang_specific
    autocmd Filetype cpp setlocal cindent
    autocmd Filetype go let g:go_highlight_structs = 1 
    autocmd Filetype go let g:go_highlight_methods = 1
    autocmd Filetype go let g:go_highlight_functions = 1
    autocmd Filetype go let g:go_highlight_operators = 1
    autocmd Filetype go let g:go_highlight_build_constraints = 1
    autocmd Filetype go let g:go_list_type = "quickfix"
    autocmd Filetype go let g:go_fmt_command = 'goimports'
    autocmd Filetype php setlocal makeprg=php\ -l\ %
    autocmd Filetype php setlocal errorformat=%m\ in\ %f\ on\ line\ %l,%-GErrors\ parsing\ %f,%-G
    autocmd Filetype php nnoremap <buffer> ø :make<cr>
    autocmd Filetype pandoc nnoremap <buffer> ø :!pandoc % -o %:r.pdf --from markdown --template eisvogel --listings<cr>
    autocmd Filetype pandoc nnoremap <buffer> Ø :!evince %:r.pdf<cr>
augroup END

"}}}
"---------------------- KOSMETISK (syntax, highlighting) --------------------{{{

syntax on

" fininstillinger på farger{{{
augroup colorscheme_greier
    autocmd!

    " colorcolumn
    autocmd ColorScheme * highlight ColorColumn ctermbg=8

    " utgangsfarga på statuslinja
    autocmd ColorScheme * highlight ntt ctermbg=0 ctermfg=2
            \ guifg=darkgreen
    autocmd ColorScheme * highlight StatusLine ctermfg=0 ctermbg=2
            \ guibg=darkgreen

    " comment greier
    autocmd ColorScheme * highlight Comment ctermfg=darkblue

    " cursorline greier. EKSPERIMENTELT
    autocmd ColorScheme * highlight CursorLineNR
            \ ctermbg=black
            \ cterm=bold,underline

augroup END
"}}}
" farge på statuslinja skifter ved modusskifte, og vimrc folding{{{
if version >= 700
    augroup statusline_group
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
        autocmd FileType i3 setlocal foldmethod=marker
        autocmd Filetype i3 setlocal foldlevelstart=0
        autocmd FileType i3 setlocal foldcolumn=2
    augroup END
endif
"}}}
" instillinger for wildmenu{{{
set wildmenu
set wildmode=longest,list
set wildignore+=*.a,*.o,*.exe
set wildignore+=*.bmp,*.gif,*.ico,*.jpg,*.png,*.pdf
set wildignore+=.DS_Store,.git,.hg,.svn
set wildignore+=*~,*.swp,*.tmp
"}}}
" bygger statuslinja (venstre side){{{
set statusline=%#Statusline#\  " så uvalgte vinduer ikke blir hvite
set statusline+=%m              " vis om filen er endret
set statusline+=%f              " stien til filen
set statusline+=\ -\            " separator
set statusline+=FileType:       " tekst
set statusline+=%y              " FileTypen til filen
set statusline+=\ \ \ %#ntt#    " spacing og start ny highlight-gruppe  (923)
set statusline+=%{NiTjueTre()}  " skriv ut 923 via funksjon
set statusline+=%#Statusline#   " endre tilbake til default

" (høyre side)
set statusline+=%=              " hopp til andre siden
set statusline+=%4l             " linja vi er på
set statusline+=/               " /
set statusline+=%-4L            " totalt antall linjer
"}}}

" setter colorscheme
colorscheme darkblue

"}}}
"----------------------------------- MAPPING --------------------------------{{{

" space som leder
let mapleader="\<space>"

" endre størrelse på vindu med piltaster
nnoremap <Up>       <C-W>+
nnoremap <Down>     <C-W>-
nnoremap <Left>     <C-W><
nnoremap <Right>    <C-W>>

" Function key -kommandoer (F1: cmd, F2: vimrc, F3: source vimrc, F4: md-mode)
nnoremap <F2>     :sp $MYVIMRC<cr>
nnoremap <F3>     :so $MYVIMRC<cr>

" open notesfile
nnoremap <F12>    :sp /home/tholok/Documents/vim_notes.md<cr>

" div. modifikasjoner til bevegelser i vim
nnoremap j gj
nnoremap k gk
inoremap jk <esc>
nnoremap Y y$

" skifte tab
nnoremap <leader><cr> gt
nnoremap <leader><leader><cr> :tabm +1<cr>
nnoremap <leader><tab> gT
nnoremap <leader><leader><tab> :tabm -1<cr>
nnoremap <leader><bs> :$tabnew 
"nnoremap <leader><leader><bs> :$tabnew<cr>:call QuickfixToggle()<cr><cr>

" øk/minske (gui) font-størrelsen
nnoremap <leader><up>       :call IncGUIFontSize(+1)<cr>
nnoremap <leader><down>     :call IncGUIFontSize(-1)<cr>

" vindu-greier:
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
nnoremap <leader><leader>w :tabonly<cr>
nnoremap <leader>q <c-w>q
nnoremap <leader><leader>q :tabclose<cr>
nnoremap <leader>f :sp 
nnoremap <leader>F :sp<cr>
nnoremap <leader><leader>f :call OpenMatchingFile(0)<cr>
nnoremap <leader>d :rightb vsp 
nnoremap <leader>D :rightb vsp<cr>
nnoremap <leader><leader>d :call OpenMatchingFile(1)<cr>
nnoremap <leader>t <c-w>r
nnoremap <leader>T <c-w>T

" søke-greier: BRUKER IKKE R OG E
nnoremap <leader>r :call SearchAndReplaceCword() <cr>
nnoremap <leader>e :call SearchAndReplaceCwordC() <cr>
nnoremap <leader>y :set hlsearch<cr>
nnoremap <leader>Y :set nohlsearch<cr>

" mark. 2 stykk. Trykk én gang for å sette, og to for å gå til
nnoremap <leader><leader>s ma
nnoremap <leader>s `a
nnoremap <leader><leader>a mb
nnoremap <leader>a `b

" vimgrep greier: TODO FIKS
"nnoremap <leader>g :set operatorfunc=<SID>GrepOperator<cr>g@
"vnoremap <leader>g :<c-u>call <SID>GrepOperator(visualmode())<cr>
nnoremap <leader>m :call QuickfixToggle()<cr>
"nnoremap <leader>n :cnext<cr>
"nnoremap <leader>p :cprevious<cr>

" ny linje under/over uten å gå inn i insert-mode
nnoremap <leader>o o<esc>
nnoremap <leader><s-o> O<esc>

" msbuild-greier (ø: bygg(-og-kjør), Ø: bygg, <leader>ø: kjør)
nnoremap <silent> ø              :GoBuild!<cr>
nnoremap <silent> Ø              :GoRun<cr>
nnoremap <silent> <leader>ø      :GoTest<cr>
nnoremap <silent> <leader>Ø      :GoCoverageBrowser<cr>

" vis/gjem trailing whitespace
nnoremap <leader>u :match Error /\v\s+$/<cr>
nnoremap <leader>i :match none<cr>

" flytt linje én opp, én ned
nnoremap <leader>+ ddp
nnoremap <leader>- ddkP

" tre ganger = ingenting
nnoremap <leader><leader><leader> <nop>

" kommenteringssnarveier og qf j,k fix{{{
augroup filetype_specific_stuff
    autocmd!
    autocmd FileType cpp nnoremap <buffer> <leader>b :call
            \ MakeTitle("//")<cr>
    autocmd FileType cpp nnoremap <buffer> <leader>c        mq0i//<esc>`qll
    autocmd FileType cpp nnoremap <buffer> <leader>x        mq0xx`qhh
    autocmd FileType cpp vnoremap <buffer> <leader>c        omq0<S-i>//<esc>`qll
    autocmd FileType cpp vnoremap <buffer> <leader>x        omq0o0lx`qhh
    autocmd FileType python nnoremap <buffer> <leader>b :call
            \ MakeTitle("#")<cr>
    autocmd FileType python nnoremap <buffer> <leader>c     mq0i#<esc>`ql
    autocmd FileType python nnoremap <buffer> <leader>x     mq0x`qh
    autocmd FileType python vnoremap <buffer> <leader>c     omq0<S-i>#<esc>`ql
    autocmd FileType python vnoremap <buffer> <leader>x     omq0o0x`qh
    autocmd FileType vim nnoremap <buffer> <leader>b :call
            \ MakeTitle("\"")<cr>
    autocmd FileType vim nnoremap <buffer> <leader>c        mq0i"<esc>`ql
    autocmd FileType vim nnoremap <buffer> <leader>x        mq0x`qh
    autocmd FileType vim vnoremap <buffer> <leader>c        omq0<S-i>"<esc>`ql
    autocmd FileType vim vnoremap <buffer> <leader>x        omq0o0x`qh
    autocmd FileType prolog nnoremap <buffer> <leader>b :call
            \ MakeTitle("%")<cr>
    autocmd FileType prolog nnoremap <buffer> <leader>c     mq0i%<esc>`ql
    autocmd FileType prolog nnoremap <buffer> <leader>x     mq0x`qh
    autocmd FileType prolog vnoremap <buffer> <leader>c     omq0<S-i>%<esc>`ql
    autocmd FileType prolog vnoremap <buffer> <leader>x     omq0o0x`qh
    autocmd FileType tex nnoremap <buffer> <leader>b :call
            \ MakeTitle("%")<cr>
    autocmd FileType tex nnoremap <buffer> <leader>c        mq0i%<esc>`ql
    autocmd FileType tex nnoremap <buffer> <leader>x        mq0x`qh
    autocmd FileType tex vnoremap <buffer> <leader>c        omq0<S-i>%<esc>`ql
    autocmd FileType tex vnoremap <buffer> <leader>x        omq0o0x`qh
    autocmd FileType go nnoremap <buffer> <leader>b         :call
            \ MakeTitle("//")<cr>
    autocmd FileType go nnoremap <buffer> <leader>c         mq0i//<esc>`qll
    autocmd FileType go nnoremap <buffer> <leader>x         mq0xx`qhh
    autocmd FileType go vnoremap <buffer> <leader>c         omq0<S-i>//<esc>`qll
    autocmd FileType go vnoremap <buffer> <leader>x         omq0o0lx`qhh
    autocmd FileType i3 nnoremap <buffer> <leader>b :call
            \ MakeTitle("#")<cr>
    autocmd FileType i3 nnoremap <buffer> <leader>c     mq0i#<esc>`ql
    autocmd FileType i3 nnoremap <buffer> <leader>x     mq0x`qh
    autocmd FileType i3 vnoremap <buffer> <leader>c     omq0<S-i>#<esc>`ql
    autocmd FileType i3 vnoremap <buffer> <leader>x     omq0o0x`qh
    autocmd FileType Dockerfile nnoremap <buffer> <leader>b :call
            \ MakeTitle("#")<cr>
    autocmd FileType Dockerfile nnoremap <buffer> <leader>c     mq0i#<esc>`ql
    autocmd FileType Dockerfile nnoremap <buffer> <leader>x     mq0x`qh
    autocmd FileType Dockerfile vnoremap <buffer> <leader>c     omq0<S-i>#<esc>`ql
    autocmd FileType Dockerfile vnoremap <buffer> <leader>x     omq0o0x`qh
    autocmd FileType qf nnoremap <buffer> j j
    autocmd FileType qf nnoremap <buffer> k k
    "autocmd FileType qf nnoremap <buffer> <bs> <C-W><cr>:call
    "        \ QuickfixToggle()<cr><C-W>T:call QuickfixToggle()<cr>
augroup END
"}}}

"}}}
"------------------------------------ POST ----------------------------------{{{

execute pathogen#infect()

"set completeopt-=preview
" tester ut denne:
""""let g:ycm_autoclose_preview_window_after_insertion = 1

"}}}

set switchbuf=usetab
set autoindent
"setlocal formatoptions=ctnqro
"setlocal comments+=n:*,n:#

vnoremap J :m '>+1<CR>gv=gv
vnoremap K :m '<-2<CR>gv=gv

set nospell

nnoremap  3
nnoremap  3

" emmet reference: https://raw.githubusercontent.com/mattn/emmet-vim/master/TUTORIAL

augroup asm_stuff
    autocmd!
    autocmd FileType asm nnoremap <buffer> ø :!make %<cr>

augroup END
