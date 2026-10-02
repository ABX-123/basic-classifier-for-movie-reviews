#include "../src/SentimentAnalyzer.h"
#include <gtest/gtest.h>

class SentimentTest : public ::testing::Test {
protected:
    static SentimentAnalyzer* analyzer;

    static void SetUpTestSuite() {
        analyzer = new SentimentAnalyzer();
    }

    static void TearDownTestSuite() {
        delete analyzer;
        analyzer = nullptr;
    }
};

SentimentAnalyzer* SentimentTest::analyzer = nullptr;

TEST_F(SentimentTest, Negation) {
	
    EXPECT_EQ(analyzer->analyze("not bad").first, "positive");
    EXPECT_EQ(analyzer->analyze("not good").first, "negative");
    EXPECT_EQ(analyzer->analyze("not that bad").first, "positive");
    EXPECT_EQ(analyzer->analyze("not that good").first, "negative");
    EXPECT_EQ(analyzer->analyze("not so bad").first, "positive");
    EXPECT_EQ(analyzer->analyze("not so good").first, "negative");
}

TEST_F(SentimentTest, ShortExpressions) {
	
	EXPECT_EQ(analyzer->analyze("nice").first, "positive");
    EXPECT_EQ(analyzer->analyze("good").first, "positive");
    EXPECT_EQ(analyzer->analyze("bad").first, "negative");
    EXPECT_EQ(analyzer->analyze("terrible").first, "negative");
	
	EXPECT_EQ(analyzer->analyze("nice movie").first, "positive");
    EXPECT_EQ(analyzer->analyze("good movie").first, "positive");
    EXPECT_EQ(analyzer->analyze("bad movie").first, "negative");
    EXPECT_EQ(analyzer->analyze("terrible movie").first, "negative");
	
    EXPECT_EQ(analyzer->analyze("well").first, "positive");
    EXPECT_EQ(analyzer->analyze("nothing new").first, "negative");
}

TEST_F(SentimentTest, LongerExpressions) {
	
    EXPECT_EQ(analyzer->analyze("I will watch it again").first, "positive");
    EXPECT_EQ(analyzer->analyze("I will watch it again and again").first, "positive");
    EXPECT_EQ(analyzer->analyze("The movie was exciting and beautifully made.").first, "positive");
    EXPECT_EQ(analyzer->analyze("I loved the story and the great acting.").first, "positive");
    EXPECT_EQ(analyzer->analyze("This film was funny, engaging, and enjoyable.").first, "positive");
    EXPECT_EQ(analyzer->analyze("It was a fantastic movie with a satisfying ending.").first, "positive");
    EXPECT_EQ(analyzer->analyze("The characters were well developed.").first, "positive");
	
	EXPECT_EQ(analyzer->analyze("I will not watch it again").first, "negative");
	EXPECT_EQ(analyzer->analyze("The movie was boring and too slow.").first, "negative");
	EXPECT_EQ(analyzer->analyze("I disliked the weak story and poor acting.").first, "negative");
	EXPECT_EQ(analyzer->analyze("The film was confusing and poorly written.").first, "negative");
	EXPECT_EQ(analyzer->analyze("The characters were uninteresting and forgettable.").first, "negative");
	EXPECT_EQ(analyzer->analyze("It was disappointing and not worth watching.").first, "negative");
}

TEST_F(SentimentTest, Paragraphs) {
	
	std::string review = "I was completely surprised by how much I enjoyed this movie. The story starts slowly, but once the characters are introduced, it becomes very engaging. The acting is excellent, especially from the lead actor, who manages to make even the quieter scenes feel meaningful. The cinematography is beautiful without being distracting, and the soundtrack fits the mood perfectly. There are a few predictable moments, but they did not bother me because the movie was so well made overall. By the end, I genuinely cared about what happened to the characters. This is one of those movies that leaves you with a good feeling after the credits roll.";
    EXPECT_EQ(analyzer->analyze(review).first, "positive");
	
	review = "This was a wonderful film from beginning to end. I went into it without expecting very much, but I quickly found myself interested in the story and the characters. The director does a great job balancing humor with more emotional moments, and the transitions between them feel natural. The supporting cast is also surprisingly strong, giving the movie much more personality than I expected. Some parts of the plot are a little familiar, but the performances make them enjoyable anyway. The final scenes were especially satisfying and gave the story a proper conclusion. I would definitely recommend this movie to anyone looking for something entertaining and heartfelt.";
    EXPECT_EQ(analyzer->analyze(review).first, "positive");
	
	review = "What an enjoyable movie. It is not a complicated story, but it does not need to be because everything is executed so well. The main character is easy to sympathize with, and the actor gives a very believable performance throughout the film. I particularly liked the way the relationship between the characters developed gradually instead of suddenly becoming perfect. The visuals are impressive, and several scenes are genuinely beautiful to watch. Even though I could predict a couple of the major events, I was still interested enough to keep watching. The movie manages to be entertaining while also having something meaningful to say about friendship and personal change.";
    EXPECT_EQ(analyzer->analyze(review).first, "positive");
	
	review = "I really liked this film and found myself thinking about it long after it ended. The story combines drama, mystery, and a little comedy without allowing any one element to overwhelm the others. The cast works extremely well together, and the dialogue feels much more natural than it does in most movies of this type. There are several memorable scenes, particularly during the second half, where the story becomes much more emotional. The pacing is mostly excellent, although the opening could have been slightly shorter. Still, that is a minor complaint about an otherwise impressive production. If you enjoy character-driven movies with an interesting story, this one is definitely worth watching.";
    EXPECT_EQ(analyzer->analyze(review).first, "positive");
	
	review = "This movie was much better than I expected. From the opening scene, it has a distinctive atmosphere that immediately made me curious about where the story was going. The performances are convincing, and the lead actress gives the character a lot of depth without overacting. I also appreciated the attention to small details in the sets and locations, which made the world feel believable. The story contains a few familiar ideas, but they are presented in a fresh enough way to remain interesting. The ending was emotional without becoming overly sentimental. Overall, this is a thoughtful and entertaining film that deserves more attention than it seems to have received.";
    EXPECT_EQ(analyzer->analyze(review).first, "positive");
	
	review = "I really wanted to enjoy this movie, but unfortunately it never became interesting. The story feels like a collection of ideas borrowed from much better films, and almost nothing develops in a convincing way. The main characters are poorly written, making it difficult to care about what happens to them. The acting is also disappointing, with several scenes feeling more like rehearsals than finished performances. The movie spends a lot of time on unnecessary conversations while rushing through events that should have been important. Even the ending feels predictable and unsatisfying. There are a couple of decent scenes, but they are not enough to rescue the movie from its many problems.";
	EXPECT_EQ(analyzer->analyze(review).first, "negative");
	
	review = "This was one of the most disappointing movies I have watched recently. The trailer made it look exciting, but almost all of the interesting material seems to have been included there. Once the movie begins, the story moves extremely slowly and spends far too much time explaining things that should have been obvious. The characters are not particularly likable or memorable, and the dialogue often sounds unnatural. I kept waiting for something significant to happen, but every potentially interesting moment was followed by another long and boring scene. The final act is especially weak and provides no satisfying conclusion. I would not recommend wasting your time with this one.";
    EXPECT_EQ(analyzer->analyze(review).first, "negative");
	
	review = "I cannot understand why this movie received so much praise. The basic idea is actually quite interesting, but the execution is terrible. The screenplay is filled with predictable dialogue and characters who make ridiculous decisions simply to move the plot forward. The lead performance is particularly disappointing because the actor seems completely disconnected from the emotional moments. The special effects are acceptable, but they cannot compensate for the weak story. Several scenes feel unnecessarily long, and the movie could easily have been shortened by thirty minutes without losing anything important. By the time the credits appeared, I was relieved that it was finally over.";
    EXPECT_EQ(analyzer->analyze(review).first, "negative");
	
	review = "This movie had everything it needed to be entertaining, but somehow managed to waste all of its potential. The opening is promising and introduces an interesting situation, but the story quickly becomes repetitive. Every character behaves exactly as you would expect, and there are almost no surprises. The humor is also very inconsistent, with jokes that feel forced rather than funny. I found myself checking how much time was left several times because the movie seemed much longer than it actually was. The final confrontation should have been exciting, but instead it was predictable and strangely emotionless. There are some attractive visuals, but there is very little substance behind them.";
    EXPECT_EQ(analyzer->analyze(review).first, "negative");
	
	review = "I had high expectations for this film after hearing positive comments about it, so perhaps that made the disappointment worse. The movie begins with an intriguing premise, but the writers never really know what to do with it. The plot becomes increasingly confusing, while the characters remain shallow and difficult to understand. Even the supposedly emotional scenes fail to have much impact because the movie has not done enough to establish believable relationships between the characters. The music is overly dramatic and often makes ordinary scenes feel more important than they are. There are a few good performances and some nice photography, but overall the film feels unfinished and unsatisfying.";
    EXPECT_EQ(analyzer->analyze(review).first, "negative");
}
