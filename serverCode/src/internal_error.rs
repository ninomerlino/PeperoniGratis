use std::{error, fmt::{Display, Debug}, sync::PoisonError};
use rocket::{Responder};

#[derive(Debug,Responder)]
pub struct Error{
    value : String,
}

pub type Result<T> = std::result::Result<T,Error>;

impl Display for Error{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        (self as &dyn Debug).fmt(f)
    }
}

impl error::Error for Error{}

impl<E> From<PoisonError<E>> for Error{
    fn from(e : PoisonError<E>) -> Self {
        Self { value: e.to_string() }
    }
}
