## replace NA/NaN/etc in vector with default

safetify <- Vectorize(function(x, default) {
  if (is.na(x)) {
    default
  } else {
    x
  }
}, vectorize.args="x")
