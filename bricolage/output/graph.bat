@echo off
for /r . %%f in (*.graph) do (
	echo|set /p=.
	dot -Tpdf -Nfontname="Monospace" %%f > %%~nf.pdf
)
echo 