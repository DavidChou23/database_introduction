select count(distinct ver) as cnt
from 
(  select substring_index(match_info.version, '.', 2) as ver 
   from match_info
) as tmp;