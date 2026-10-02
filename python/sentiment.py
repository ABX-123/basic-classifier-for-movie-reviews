from pathlib import Path
import re

import joblib


# =========================================================
# Paths
# =========================================================

MODEL_PATH = Path(
    "/app/model/sentiment_model.joblib"
)


# =========================================================
# Load trained model
# =========================================================

_model_data = joblib.load(MODEL_PATH)

_vectorizer = _model_data["vectorizer"]
_model = _model_data["model"]


# =========================================================
# Text preprocessing
# =========================================================

def normalize_negation(text):
    """
    Preserve the original words while adding explicit
    features for words appearing after negation.
    """

    text = text.lower()

    tokens = re.findall(
        r"[a-z']+",
        text,
    )

    result = list(tokens)

    negation_words = {
        "not",
        "never",
        "no",
        "cannot",
        "can't",
        "don't",
        "doesn't",
        "didn't",
    }

    ignored_after_negation = {
        "the",
        "a",
        "an",
        "that",
        "so",
        "very",
    }

    negation_active = False
    remaining = 0

    for token in tokens:

        if token in negation_words:
            negation_active = True
            remaining = 3
            continue

        if (
            negation_active
            and token not in ignored_after_negation
        ):
            result.append(
                f"NOT_{token}"
            )

            remaining -= 1

            if remaining <= 0:
                negation_active = False

    return " ".join(result)


# =========================================================
# Public API used by C++
# =========================================================

def analyze(text):

    normalized = normalize_negation(text)

    X = _vectorizer.transform(
        [normalized]
    )

    prediction = _model.predict(X)[0]

    probability = (
        _model.predict_proba(X)[0].max()
    )

    sentiment = (
        "positive"
        if prediction == 1
        else "negative"
    )

    return sentiment, float(probability)

