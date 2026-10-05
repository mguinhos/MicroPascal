program Logica;
var
  a, b, c : integer;
begin
  a := 1;
  b := 2;
  c := 3;
  if not a = b and c <> a or a <= c then
    write('o')
  ;
  while a < b do
    a := a + 1
  ;
end.
