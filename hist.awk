
BEGIN {
  if (delim == "") delim = "^"
  e_min = 9999;
  e_max = 0;
  sum = 0;
}

{
  e = int(($colonne) / largeur);
  hist[e] = hist[e] + 1;
  if (e < e_min) {e_min = e};
  if (e > e_max) {e_max = e};
#  printf("e = %u ; hist[e] = %u ; e_min = %u ; e_max = %u\n", e, hist[e], e_min, e_max);
}

END {
  for (e = e_min; e <= e_max; e++)
  {
    printf("%u %u\n", e, hist[e]);
  }
}

