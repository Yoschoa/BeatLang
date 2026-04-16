#include "../../includes/ast/ast.hpp"
#include "../../includes/ast/visitor.hpp"

namespace beatlang::ast {


    void TempoNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }

    void TrackNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }

    void PatternNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }

    void PlayNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }

    void LoopNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }

    void SongNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }

    void ProgramNode::accept(Visitor& visitor) {
        visitor.visit(this);
    }
 
    void PatternNode::addTrack(std::unique_ptr<TrackNode> track) {
        patternTracks.push_back(std::move(track));
    }

 
    void LoopNode::addStatement(std::unique_ptr<StatementNode> statement) {
        body.push_back(std::move(statement));
    }

    void SongNode::buildSong(std::unique_ptr<StatementNode> statement) {
        statements.push_back(std::move(statement));
    };


}