void
func_11176c (uint32_t index, uint32_t arg1)
{
  AIL_lock ();

  *(data_159f58 + index) = arg1;
  *(data_159f18 + index) = 0; /* 32bit */

  func_110feb ();

  AIL_unlock ();
}
