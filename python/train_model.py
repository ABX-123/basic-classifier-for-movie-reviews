import re
from pathlib import Path

import joblib

from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression


# =========================================================
# Paths
# =========================================================

DATA_DIR = Path("/app/data/aclImdb")
MODEL_PATH = Path("/app/model/sentiment_model.joblib")


# =========================================================
# Load IMDb reviews
# =========================================================

def load_reviews(directory):
    texts = []
    labels = []

    for label_name, label in [("neg", 0), ("pos", 1)]:
        folder = directory / label_name

        for file in folder.glob("*.txt"):
            texts.append(
                file.read_text(encoding="utf-8")
            )
            labels.append(label)

    return texts, labels


# =========================================================
# Additional short-sentence examples
#
# IMDb mostly contains long reviews.
# These examples help the model with short expressions,
# recommendations, and negation.
# =========================================================

SHORT_EXAMPLES = [

    # Positive
    ("nice", 1),
    ("good", 1),
    ("great", 1),
    ("excellent", 1),
    ("wonderful", 1),
    ("well", 1),
    ("nice movie", 1),
    ("good movie", 1),
    ("great movie", 1),
    ("excellent movie", 1),
    ("a nice movie", 1),
    ("a good movie", 1),
    ("a great movie", 1),
    ("not bad", 1),
    ("not that bad", 1),
    ("not so bad", 1),
    ("I will watch it again", 1),
    ("I will watch it again and again", 1),
    ("ok", 1),
    ("is ok", 1),
    ("okay", 1),
    ("it is okay", 1),
    ("it is ok", 1),
    ("okay movie", 1),
    ("ok movie", 1),
    ("watch it quickly", 1),
    ("I recommend this movie", 1),
    ("I would watch this again", 1),

    
    # Negative
    ("bad", 0),
    ("terrible", 0),
    ("awful", 0),
    ("horrible", 0),
    ("boring", 0),
    ("bad movie", 0),
    ("terrible movie", 0),
    ("awful movie", 0),
    ("a bad movie", 0),
    ("a terrible movie", 0),
    ("not good", 0),
    ("not that good", 0),
    ("not so good", 0),
    ("it is not ok", 0),
    ("is not ok", 0),
    ("is not okay", 0),
    ("not ok", 0),
    ("it is not okay", 0),
    ("nothing new", 0),
    ("nothing interesting", 0),
    
    
    # A slightly longer expressions
    #("interesting characters", 1),
    #("well developed characters", 1),
    #("interesting and well developed", 1),
    #("the characters were interesting", 1),
    #("the characters were well developed", 1),
    ("very interesting", 1),
    ("the characters were interesting and well developed", 1)
    #
    #("the story looks like a copy of other movies", 0),
    #("the story is copied from other movies", 0),
    #("I do not encourage you to watch it", 0),
    #("I will never watch it", 0),
    #("I will never watch it again", 0),
    #("I do not want to watch it again", 0)
]


# =========================================================
# Negation-aware preprocessing
# =========================================================

def normalize_negation(text):
    """
    Preserve the original words while adding explicit
    features for words appearing after negation.

    Example:

        "not good"
        -> "not good NOT_good"

        "not bad"
        -> "not bad NOT_bad"
    """

    text = text.lower()

    tokens = re.findall(r"[a-z']+", text)

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
            result.append(f"NOT_{token}")

            remaining -= 1

            if remaining <= 0:
                negation_active = False

    return " ".join(result)


# =========================================================
# Prepare training data
# =========================================================

def prepare_training_data():
    print("Loading IMDb training data...")

    train_texts, train_labels = load_reviews(
        DATA_DIR / "train"
    )

    print(
        f"Loaded {len(train_texts)} IMDb reviews."
    )

    short_texts = [
        text for text, _ in SHORT_EXAMPLES
    ]

    short_labels = [
        label for _, label in SHORT_EXAMPLES
    ]

    train_texts.extend(short_texts)
    train_labels.extend(short_labels)

    train_texts = [
        normalize_negation(text)
        for text in train_texts
    ]

    return train_texts, train_labels


# =========================================================
# Create vectorizer
# =========================================================

def create_vectorizer():
    return TfidfVectorizer(
        lowercase=True,
        stop_words="english",
        max_features=25000,
        ngram_range=(1, 4),
        sublinear_tf=True,
    )


# =========================================================
# Create classifier
# =========================================================

def create_model():
    return LogisticRegression(
        max_iter=2000,
        C=2.0
    )


# =========================================================
# Train
# =========================================================

def train():

    train_texts, train_labels = (
        prepare_training_data()
    )

    print("Creating TF-IDF vectorizer...")

    vectorizer = create_vectorizer()

    X_train = vectorizer.fit_transform(
        train_texts
    )

    # IMDb examples have normal weight.
    # Curated short examples have greater influence.
    sample_weights = [1.0] * len(train_texts)

    number_of_short_examples = len(
        SHORT_EXAMPLES
    )

    for i in range(
        len(sample_weights)
        - number_of_short_examples,
        len(sample_weights),
    ):
        sample_weights[i] = 8.0

    print("Training classifier...")

    model = create_model()

    model.fit(
        X_train,
        train_labels,
        sample_weight=sample_weights,
    )

    return vectorizer, model


# =========================================================
# Save trained model
# =========================================================

def save_model(vectorizer, model):

    MODEL_PATH.parent.mkdir(
        parents=True,
        exist_ok=True,
    )

    model_data = {
        "vectorizer": vectorizer,
        "model": model,
    }

    joblib.dump(
        model_data,
        MODEL_PATH,
    )

    print(
        f"Model saved to: {MODEL_PATH}"
    )


# =========================================================
# Main
# =========================================================

if __name__ == "__main__":

    vectorizer, model = train()

    save_model(
        vectorizer,
        model,
    )

    print("Training complete.")

