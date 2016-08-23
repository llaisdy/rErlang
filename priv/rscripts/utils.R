## R utils

## safetify: replace NA/NaN/etc in vector with default
## params: vector, default
## return: new vector with NAs replaced by default

safetify <- function(x, default) {
  y <- x
  y[is.na(y)] <- default
  y
}
